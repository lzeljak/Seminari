DROP TABLE IF EXISTS AranzmanZaposlenik CASCADE;
DROP TABLE IF EXISTS AranzmanTurist CASCADE;
DROP TABLE IF EXISTS Uplata CASCADE;
DROP TABLE IF EXISTS Aranzman CASCADE;
DROP TABLE IF EXISTS Destinacija CASCADE;
DROP TABLE IF EXISTS Smjestaj CASCADE;
DROP TABLE IF EXISTS Prijevoz CASCADE;
DROP TABLE IF EXISTS Zaposlenik CASCADE;
DROP TABLE IF EXISTS Turist CASCADE;

CREATE TABLE Destinacija
(
 ID_destinacije INTEGER PRIMARY KEY,
 mjesto TEXT,
 drzava TEXT
);

CREATE TABLE Smjestaj
(
 ID_smjestaja INTEGER PRIMARY KEY,
 ime_objekta TEXT,
 adresa TEXT,
 ocjena NUMERIC CHECK (ocjena >= '1' AND ocjena <= '5')
);

CREATE TABLE Prijevoz
(
 ID_prijevoza INTEGER PRIMARY KEY,
 tip_prijevoza TEXT,
 prijevoznik TEXT
);

CREATE TABLE Aranzman
(
 ID_aranzmana INTEGER PRIMARY KEY,
 ID_destinacije INTEGER REFERENCES Destinacija(ID_destinacije),
 ID_smjestaja INTEGER REFERENCES Smjestaj(ID_smjestaja),
 ID_prijevoza INTEGER REFERENCES Prijevoz(ID_prijevoza),
 naziv TEXT,
 cijena INTEGER,
 kapacitet INTEGER,
 datum_polaska TIMESTAMP,
 datum_povratka TIMESTAMP
);

CREATE TABLE Zaposlenik
(
 ID_zaposlenika INTEGER PRIMARY KEY,
 ime TEXT,
 prezime TEXT,
 kontakt TEXT
);

CREATE TABLE Turist
(
 OIB_turista INTEGER PRIMARY KEY,
 ime TEXT,
 prezime TEXT,
 kontakt TEXT
);

CREATE TABLE Uplata
(
 Broj_uplate INTEGER PRIMARY KEY,
 OIB_turista INTEGER REFERENCES Turist(OIB_turista),
 ID_aranzmana INTEGER REFERENCES Aranzman(ID_aranzmana),
 iznos INTEGER,
 opis TEXT,
 datum TIMESTAMP
);

CREATE TABLE AranzmanTurist
(
 ID_aranzmana INTEGER REFERENCES Aranzman(ID_aranzmana),
 OIB_turista INTEGER REFERENCES Turist(OIB_turista),
 status TEXT,
 datum_rezervacije TIMESTAMP,
 PRIMARY KEY (ID_aranzmana, OIB_turista)
);

CREATE TABLE AranzmanZaposlenik
(
 ID_aranzmana INTEGER REFERENCES Aranzman(ID_aranzmana),
 ID_zaposlenika INTEGER REFERENCES Zaposlenik(ID_zaposlenika),
 opis TEXT
);





INSERT INTO Destinacija
VALUES
('1', 'Antalija', 'Turska'),
('2', 'Budva', 'Crna Gora'),
('3', 'Copacabana', 'Brazil'),
('4', 'Milano', 'Italija'),
('5', 'Kairo', 'Egipat');

INSERT INTO Smjestaj
VALUES
('01', 'Tuvana hotel', 'Tuzcular Mah. Karanlik Sok. No 18 Kaleici, Antalya 07100 Türkiye', '4.3'),
('02', 'Hotel Imperial Comference & Spa', ' 7VP3+R25, Jadranski put, Budva 85310, Crna Gora', '4.8'),
('03', 'Copacabana Palace', 'Av. Atlântica, 1702 - Copacabana, Rio de Janeiro - RJ, 22021-001, Brazil', '4.8'),
('04', 'Hotel Sabatino', 'Via Giovanni Lulli, 6, 20131 Milano MI, Italija', '3.0'),
('05', 'Ramses Hilton', '1115 Corniche El Nile, Cairo', '3.9');

INSERT INTO Prijevoz
VALUES
('001', 'Avion Airbus A320', 'Ryanair'),
('002', 'Avion Boeing 737', 'Ryanair'),
('003', 'Autobus Mercedes-Benz Tuorismo', 'XYZtours');

INSERT INTO Aranzman
VALUES
('123', '1', '01', '001', 'Tjedan u Turskoj', '600', '180', '2025-05-10 08:00:00', '2025-05-16 20:00:00'),
('456', '3', '03', '002', 'Putovanje u Brazil', '1600', '200', '2025-06-21 06:00:00', '2025-06-30 18:00:00'),
('789', '2', '02', '003', 'Izlet u Budvu', '120', '80', '2025-07-01 07:00:00', '2025-07-07 19:00:00'),
('1011', '5', '05', '002', 'Put prema piramidama', '800', '200', '2024-09-02 07:00:00', '2024-09-14 22:00:00'),
('1213', '4', '04', '001', 'Provod u Milanu', '150', '180', '2024-08-27 05:00:00', '2024-08-31 17:00:00'),
('1415', '1', '01', '002', 'Tjedan u Turskoj', '450', '180', '2024-12-06 08:00:00', '2024-12-12 20:00:00');

INSERT INTO Zaposlenik
VALUES
('14269', 'Ivan', 'Ivanovic', 'iivanovic@gmail.com'),
('90816', 'Marta', 'Miric', 'miricm123@gmail.com'),
('21220', 'Marko', 'Horvat', 'mhorva@gmail.com');

INSERT INTO Turist
VALUES
('901829555', 'Nikola', 'Bilic', '+385 99 222 0200'),
('142300765', 'Marin', 'Matic', 'mmatic@gmail.com'),
('332407681', 'Martina', 'Horvat', '+387 91 345 6782');

INSERT INTO Uplata
VALUES
('000111', '142300765', '789', '120', 'Put u Budvu', '2025-03-01 14:00:00'),
('000222', '901829555', '1415', '450', 'Putovanje u Antaliju', '2024-12-02 12:00:00'),
('000333', '332407681', '456', '1600', 'Putovanje u Brazil', '2025-01-12 11:30:00'),
('000444', '142300765', '123', '200', 'Prva rata za put u Tursku', '2025-02-16 13:20:00'),
('000555', '142300765', '123', '200', 'Druga rata za put u Tursku', '2025-02-22 10:00:00');

INSERT INTO AranzmanTurist
VALUES
('789', '142300765', 'Placeno', '2025-02-27 14:20:00'),
('123', '142300765', 'Placanje u toku', '2025-02-01 08:00:00'),
('1011', '901829555', 'Placeno', '2024-06-01 11:35:00'),
('1415', '901829555', 'Placeno', '2024-11-05 21:10:00'),
('456', '332407681', 'Placeno', '2024-12-01 15:15:00'),
('1213', '332407681', 'Placeno', '2024-03-03 12:00:00');

INSERT INTO AranzmanZaposlenik
VALUES
('1011', '21220', 'Voditelj'),
('123', '14269', 'Pomocnik voditelja'),
('123', '90816', 'Voditelj'),
('1213', '90816', 'Voditelj'),
('789', '21220', 'Voditelj');





/* Pregled putovanja pojedinog turista */
SELECT d.mjesto, d.drzava, a.datum_polaska, a.datum_povratka, at.status
FROM AranzmanTurist at
JOIN Turist t ON at.OIB_turista = t.OIB_turista
JOIN Aranzman a ON at.ID_aranzmana = a.ID_aranzmana
JOIN Destinacija d ON a.ID_destinacije = d.ID_destinacije
WHERE t.ime='Nikola' AND t.prezime='Bilic'
ORDER BY a.datum_polaska DESC;

/* Pregled putovanja pojedinog zaposlenika */
SELECT az.opis, d.mjesto, d.drzava, a.datum_polaska, a.datum_povratka
FROM AranzmanZaposlenik az
JOIN Zaposlenik z ON az.ID_zaposlenika = z.ID_zaposlenika
JOIN Aranzman a ON az.ID_aranzmana = a.ID_aranzmana
JOIN Destinacija d ON a.ID_destinacije = d.ID_destinacije
WHERE z.ime='Marko' AND z.prezime='Horvat'
ORDER BY a.datum_polaska DESC;

/* Pregled ponude pojedinog aranzmana */
SELECT a.naziv, d.mjesto, d.drzava, s.ime_objekta, p.tip_prijevoza, p.prijevoznik, a.datum_polaska, a.datum_povratka, a.kapacitet, a.cijena
FROM Aranzman a
JOIN Destinacija d ON a.ID_destinacije = d.ID_destinacije
JOIN Smjestaj s ON a.ID_smjestaja = s.ID_smjestaja
JOIN Prijevoz p ON a.ID_prijevoza = p.ID_prijevoza
WHERE a.naziv ILIKE '%tursk%';

/* Sve uplate pojedinog turista */
SELECT u.Broj_uplate, u.iznos, u.opis, u.datum
FROM Uplata u
JOIN Turist t ON u.OIB_turista = t.OIB_turista
WHERE t.ime='Marin' AND t.prezime='Matic'
ORDER BY u.datum DESC;

/* Sve uplate za pojedini aranzman */
SELECT u.Broj_uplate, u.iznos, u.opis, u.datum
FROM Uplata u
JOIN Aranzman a ON u.ID_aranzmana = a.ID_aranzmana
WHERE a.naziv ILIKE '%brazil%'
ORDER BY u.datum DESC;

/* Prihodi agencije */
CREATE VIEW PrihodiAgencije AS
SELECT SUM(iznos) AS prihodi FROM Uplata;