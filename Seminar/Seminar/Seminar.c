#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>
#define _CRT_SECURE_NO_WARNINGS
#define MAX_SIZE 256

typedef struct _song* SongPtr;
typedef struct _song {
	char name[MAX_SIZE];
	char artist[MAX_SIZE];
	double duration;
}Song;

typedef struct _playlist* PlaylistPtr;
typedef struct _playlist {
	SongPtr song;
	PlaylistPtr next;
	PlaylistPtr prev;
}Playlist;

typedef struct _lists* ListsPtr;
typedef struct _lists {
	char name[MAX_SIZE];
	PlaylistPtr playlist;
	ListsPtr next;
	ListsPtr prev;
}Lists;

SongPtr createSong(char*);
bool findByName(char*, char*);
bool findByArtist(char*, char*);
void options(ListsPtr, char*, char*);
void createPlaylist(ListsPtr, char*, char*);
void addPlaylist(ListsPtr, char*);
void removePlaylist(ListsPtr, char*);
void addSong(PlaylistPtr, char*, char*);
void removeSong(PlaylistPtr, char*);
void organizeByArtist(PlaylistPtr, char*, char*);
void organizeRandom(PlaylistPtr, char*);
void playInOrder(PlaylistPtr);
void playInReverse(PlaylistPtr);
void playRandom(PlaylistPtr);
void freeList(PlaylistPtr);
void freeAll(ListsPtr);



int main()
{
	Lists head;
	head.playlist = NULL;
	head.next = NULL;
	head.prev = NULL;
	int k = 1;
	char name[MAX_SIZE];
	while (k!=0) {
		printf("Odaberite:\n  1 za odabiranje postojece playliste\n  2 za dodavanje nove playliste\n  3 za brisanje playliste\n  0 za izlaz\n\n");
		scanf("%d", &k);
		switch (k) {
		case 0:
			break;
		case 1:
			printf("Unesite ime playliste\n");
			getchar();
			scanf("%255[^\n]", name);
			options(&head, name, "pjesme.txt");
			break;
		case 2:
			printf("Unesite ime playliste\n");
			getchar();
			scanf("%255[^\n]", name);
			createPlaylist(&head, name, "pjesme.txt");
			break;
		case 3:
			printf("Unesite ime playliste\n");
			getchar();
			scanf("%255[^\n]", name);
			removePlaylist(&head, name);
			break;
		default:
			printf("Unijeli ste nepostojecu opciju\n");
			break;
		}
		if (k == 0)
			break;
	}

	freeAll(&head);

	return 0;
}



SongPtr createSong(char* line)
{
	SongPtr newSong = (SongPtr)malloc(sizeof(Song));
	char name[MAX_SIZE];
	char artist[MAX_SIZE];
	double duration;
	int numBytes;
	char* ptr = line;
	if (sscanf(ptr, "%255[^\-]- %n", name, &numBytes) == 1) {
		strcpy(newSong->name, name);
		ptr += numBytes;
	}
	if (sscanf(ptr, "%255[^\-]- %n", artist, &numBytes)==1) {
		strcpy(newSong->artist, artist);
		ptr += numBytes;
	}
	if (sscanf(ptr, "%.2lf", &duration))
		newSong->duration = duration;
	return newSong;
}

bool findByName(char* line, char* name)
{
	char tempName[MAX_SIZE];
	if (sscanf(line, "%255[^\-]", tempName) == 1) {
		if (strcmp(tempName, name) == 0)
			return true;
		else
			return false;
	}
	else
		return false;
}

bool findByArtist(char* line, char* artist)
{
	char tempName[MAX_SIZE];
	char tempArtist[MAX_SIZE];
	char* ptr = line;
	int numBytes;
	if (sscanf(ptr, "%255[^\-]-%n", tempName, &numBytes) == 1)
		ptr += numBytes;
	if (sscanf(ptr, "%[^\-]", tempArtist) == 1) {
		if (strcmp(tempArtist, artist) == 0)
			return true;
		else
			return false;
	}
	else
		return false;
}

void options(ListsPtr head, char* name, char* filePath)
{
	ListsPtr current;
	ListsPtr temp = head->next;
	while (temp != NULL && strcmp(temp->name, name) != 0)
		temp = temp->next;
	if (temp)
		current = temp;
	else {
		printf("Playlist not found\n\n");
		return;
	}
	current->playlist->prev = NULL;
	int k=1, m;
	char songName[MAX_SIZE];
	while (k!=0) {
		printf("  1 za sviranje\n  2 za dodavanje pjesama\n  3 za brisanje pjesama\n  0 za izlaz\n\n");
		scanf("%d", &k);
		if (k == 0)
			break;
		switch (k) {
		case 1:
			printf("Zelite li svirati:\n  1 po redu\n  2 obrnutim redoslijedom\n  3 nasumicnim redoslijedom\n");
			scanf("%d", &m);
			if (m == 1)
				playInOrder(current->playlist);
			else if (m == 2)
				playInReverse(current->playlist);
			else if (m == 3)
				playRandom(current->playlist);
			else
				printf("Unijeli ste nepostojecu opciju\n");
			break;
		case 2:
			printf("Unesite pjesmu koju zelite dodati\n");
			getchar();
			scanf("%255[^\n]", songName);
			addSong(current->playlist, songName, filePath);
			break;
		case 3:
			printf("Unesite pjesmu koju zelite izbrisati\n");
			getchar();
			scanf("%255[^\n]", songName);
			removeSong(current->playlist, songName);
			break;
		default:
			printf("Unijeli ste nepostojecu opciju\n");
			break;
		}
	}
}

void createPlaylist(ListsPtr head, char* name, char* filePath)
{
	ListsPtr temp = head->next;
	while (temp != NULL) {
		if (strcmp(temp->name, name) == 0) {
			printf("Playlist with that name already exists\n\n");
			return;
		}
		temp = temp->next;
	}
	addPlaylist(head, name);
	ListsPtr current;
	temp = head->next;
	while (temp != NULL && strcmp(temp->name, name) != 0)
		temp = temp->next;
	if (temp)
		current = temp;
	else {
		printf("Error finding playlist\n\n");
		return;
	}
	int k;
	char songName[MAX_SIZE], artist[MAX_SIZE];
	bool state = true;
	while (state) {
		printf("  1 za dodavanje pjesama\n  2 za stvaranje playliste prema autoru\n  3 za stvaranje nasumicne playliste\n\n");
		scanf("%d", &k);
		switch (k) {
		case 1:
			while (1) {
				printf("Unesite pjesmu koju zelite dodati\n");
				getchar();
				if (scanf("%255[^\n]", songName) != 1)
					break;
				else
					addSong(current->playlist, songName, filePath);
			}
			state = false;
			break;
		case 2:
			printf("Unesite ime autora\n");
			getchar();
			scanf("%255[^\n]", artist);
			organizeByArtist(current->playlist, artist, filePath);
			state = false;
			break;
		case 3:
			organizeRandom(current->playlist, filePath);
			state = false;
			break;
		default:
			printf("Unijeli ste nepostojecu opciju\n");
			break;
		}
	}
}

void addPlaylist(ListsPtr head, char* name)
{
	ListsPtr temp = head;
	ListsPtr new = (ListsPtr)malloc(sizeof(Lists));
	if (new == NULL) {
		printf("Error creating new playlist\n\n");
		return;
	}
	strcpy(new->name, name);
	new->playlist = (PlaylistPtr)malloc(sizeof(Playlist));
	if (new->playlist == NULL) {
		printf("Error creating new playlist 2\n\n");
		return;
	}
	new->playlist->next = NULL;
	while (temp->next != NULL)
		temp = temp->next;
	new->next = temp->next;
	temp->next = new;
	new->prev = temp;
}

void removePlaylist(ListsPtr head, char* name)
{
	if (head->next == NULL) {
		printf("You don't have any playlists\n\n");
		return;
	}
	ListsPtr temp = head->next;
	while (temp != NULL && strcmp(temp->name, name) != 0)
		temp = temp->next;
	if (temp) {
		ListsPtr toDelete = temp;
		temp = head;
		while (temp->next != toDelete)
			temp = temp->next;
		temp->next = toDelete->next;
		if (toDelete->next != NULL)
			toDelete->next->prev = temp;
		free(toDelete);
		printf("Playlist removed successfully\n\n");
	}
	else
		printf("Playlist not found\n\n");
}

void addSong(PlaylistPtr head, char* name, char* filePath)
{
	PlaylistPtr temp = head->next;
	while (temp != NULL) {
		if (strcmp(temp->song->name, name) == 0) {
			printf("This song was already added\n\n");
			return;
		}
		else
			temp = temp->next;
	}
	FILE* file = fopen(filePath, "r");
	if (file == NULL) {
		printf("Error opening file\n\n");
		return;
	}
	temp = head;
	char line[MAX_SIZE];
	bool isFound = false;
	while (fgets(line, sizeof(line), file)) {
		line[strcspn(line, "\n")] = '\0';
		if (findByName(line, name)) {
			PlaylistPtr new = (PlaylistPtr)malloc(sizeof(Playlist));
			if (new == NULL) {
				printf("Error allocating memory for a new song\n\n");
				fclose(file);
				return;
			}
			new->song = createSong(line);
			if (new->song == NULL) {
				printf("Error adding song\n\n");
				fclose(file);
				return;
			}
			while (temp->next != NULL)
				temp = temp->next;
			new->next = temp->next;
			temp->next = new;
			new->prev = temp;
			isFound = true;
			break;
		}
	}
	if (isFound)
		printf("Song added successfully\n\n");
	else
		printf("Song not found\n\n");
	fclose(file);
}

void removeSong(PlaylistPtr head, char* name)
{
	if (head->next == NULL) {
		printf("Playlist is empty\n\n");
		return;
	}
	PlaylistPtr temp = head->next;
	while (temp != NULL && strcmp(temp->song->name, name) != 0)
		temp = temp->next;
	if (temp) {
		PlaylistPtr toDelete = temp;
		temp = head;
		while (temp->next != toDelete)
			temp = temp->next;
		temp->next = toDelete->next;
		if (toDelete->next != NULL)
			toDelete->next->prev = temp;
		free(toDelete);
		printf("Song removed successfully\n\n");
	}
	else
		printf("Song not found\n\n");
}

void organizeByArtist(PlaylistPtr head, char* artist, char* filePath)
{
	FILE* file = fopen(filePath, "r");
	if (file == NULL) {
		printf("Error opening file\n\n");
		return;
	}
	char line[MAX_SIZE];
	PlaylistPtr temp = head;
	bool isFound = false;
	while (fgets(line, sizeof(line), file)) {
		line[strcspn(line, "\n")] = '\0';
		if (findByArtist(line, artist)) {
			PlaylistPtr new = (PlaylistPtr)malloc(sizeof(Playlist));
			if (new == NULL) {
				printf("Error allocating memory for a new song\n\n");
				fclose(file);
				return;
			}
			new->song = createSong(line);
			if (new->song == NULL) {
				printf("Error adding song\n\n");
				fclose(file);
				return;
			}
			while (temp->next != NULL)
				temp = temp->next;
			new->next = temp->next;
			temp->next = new;
			new->prev = temp;
			isFound = true;
		}
	}
	if (isFound)
		printf("Playlist organized successfully\n\n");
	else
		printf("Artist not found\n\n");
	fclose(file);
}

void organizeRandom(PlaylistPtr head, char* filePath)
{
	FILE* file = fopen(filePath, "r");
	if (file == NULL) {
		printf("Error opening file\n\n");
		return;
	}
	int i=0, size=0;
	char tempName[MAX_SIZE], line[1024];
	while (fgets(line, sizeof(line), file))
		size++;
	if (size == 0) {
		printf("File is empty\n\n");
		return;
	}
	rewind(file);
	char** temp = (char**)malloc(size * sizeof(char*));
	if (temp == NULL) {
		printf("Error while organizing randomly\n\n");
		fclose(file);
		return;
	}
	while (fgets(line, sizeof(line), file)) {
		line[strcspn(line, "\n")] = '\0';
		if (sscanf(line, "%255[^\-]", tempName) != 1)
			printf("Error reading lines\n\n");
		temp[i] = (char*)malloc(strlen(tempName) + 1);
		if (temp[i] == NULL) {
			printf("Error while organizing randomly 2\n\n");
			fclose(file);
			return;
		}
		strcpy(temp[i], tempName);
		i++;
	}
	srand(time(NULL));
	int randomNumber = rand() % size + 1;
	for (i = 0;i < randomNumber;i++) {
		int randomIndex = rand() % size;
		addSong(head, temp[randomIndex], filePath);
	}
	if (head->next != NULL)
		printf("Playlist generated successfully\n\n");
	else
		printf("Error generating random playlist\n\n");
	for (i = 0;i < size;i++)
		free(temp[i]);
	free(temp);
	fclose(file);
}

void playInOrder(PlaylistPtr head)
{
	if (head->next == NULL) {
		printf("Playlist is empty\n\n");
		return;
	}
	PlaylistPtr temp = head->next;
	while (temp != NULL) {
		printf("'%s' by %s\n\n", temp->song->name, temp->song->artist);
		temp = temp->next;
	}
}

void playInReverse(PlaylistPtr head)
{
	if (head->next == NULL) {
		printf("Playlist is empty\n\n");
		return;
	}
	PlaylistPtr temp = head;
	while (temp->next != NULL) {
		temp = temp->next;
	}
	while (temp->prev!=NULL) {
		printf("'%s' by %s\n\n", temp->song->name, temp->song->artist);
		temp = temp->prev;
	}
}

void playRandom(PlaylistPtr head)
{
	PlaylistPtr temp = head->next;
	int size = 0;
	while (temp != NULL) {
		size++;
		temp = temp->next;
	}
	if (size == 0) {
		printf("Playlist is empty\n\n");
		return;
	}
	char** names = (char**)malloc(size * sizeof(char*));
	char** artists = (char**)malloc(size * sizeof(char*));
	double* durations = (double*)malloc(size * sizeof(double));
	temp = head->next;
	int i=0;
	while (temp != NULL) {
		names[i] = temp->song->name;
		artists[i] = temp->song->artist;
		durations[i] = temp->song->duration;
		temp = temp->next;
		i++;
	}
	srand(time(NULL));
	for (i = size - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		char* tempName = names[i];
		names[i] = names[j];
		names[j] = tempName;
		char* tempArtist = artists[i];
		artists[i] = artists[j];
		artists[j] = tempArtist;
		double tempDuration = durations[i];
		durations[i] = durations[j];
		durations[j] = tempDuration;
	}
	for (i = 0;i < size;i++) {
		printf("'%s' by %s\n\n", names[i], artists[i]);
	}
	free(names);
	free(artists);
	free(durations);
}

void freeList(PlaylistPtr head)
{
	PlaylistPtr temp = head->next;
	while (temp != NULL) {
		free(temp->song);
		PlaylistPtr next = temp->next;
		free(temp);
		temp = next;
	}
	free(head);
}

void freeAll(ListsPtr head)
{
	ListsPtr temp = head->next;
	while (temp != NULL) {
		freeList(temp->playlist);
		ListsPtr next = temp->next;
		free(temp);
		temp = next;
	}
}