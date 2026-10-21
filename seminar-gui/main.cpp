#include <QMainWindow>
#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QPoint>
#include <QApplication>
#include <QMenuBar>
#include <QCloseEvent>
#include <QMessageBox>

class Shape {
public:
    enum Type { Rectangle, Triangle, Circle };

    Shape(Type type, const QPoint &position)
        : m_type(type), m_position(position) {}

    virtual void draw(QPainter &painter) const = 0;
    virtual QString saveToString() const = 0;

    Type type() const { return m_type; }

protected:
    Type m_type;
    QPoint m_position;
};

class RectangleShape : public Shape {
public:
    RectangleShape(const QPoint &position)
        : Shape(Rectangle, position) {}

    void draw(QPainter &painter) const override {
        painter.drawRect(m_position.x() - 25, m_position.y() - 25, 50, 50);
    }

    QString saveToString() const override {
        return QString("Rectangle %1 %2").arg(m_position.x()).arg(m_position.y());
    }
};

class TriangleShape : public Shape {
public:
    TriangleShape(const QPoint &position)
        : Shape(Triangle, position) {}

    void draw(QPainter &painter) const override {
        QPolygon polygon;
        polygon << QPoint(m_position.x(), m_position.y() - 25)
                << QPoint(m_position.x() - 25, m_position.y() + 25)
                << QPoint(m_position.x() + 25, m_position.y() + 25);
        painter.drawPolygon(polygon);
    }

    QString saveToString() const override {
        return QString("Triangle %1 %2").arg(m_position.x()).arg(m_position.y());
    }
};

class CircleShape : public Shape {
public:
    CircleShape(const QPoint &position)
        : Shape(Circle, position) {}

    void draw(QPainter &painter) const override {
        painter.drawEllipse(m_position.x() - 25, m_position.y() - 25, 50, 50);
    }

    QString saveToString() const override {
        return QString("Circle %1 %2").arg(m_position.x()).arg(m_position.y());
    }
};

class ShapeDrawer : public QMainWindow {
    Q_OBJECT

public:
    ShapeDrawer(QWidget *parent = nullptr) : QMainWindow(parent), currentShapeType(Shape::Rectangle) {
        setWindowTitle("Shape Drawer");
        setFixedSize(800, 600);

        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        QAction *changeShapeAction = new QAction("Promjeni oblik", this);
        connect(changeShapeAction, &QAction::triggered, this, &ShapeDrawer::changeShape);

        QAction *saveShapesAction = new QAction("Spremi", this);
        connect(saveShapesAction, &QAction::triggered, this, &ShapeDrawer::saveShapes);

        QAction *loadShapesAction = new QAction("Ucitaj", this);
        connect(loadShapesAction, &QAction::triggered, this, &ShapeDrawer::loadShapes);

        QMenu *editMenu = menuBar()->addMenu("Edit");
        editMenu->addAction(changeShapeAction);
        editMenu->addAction(saveShapesAction);
        editMenu->addAction(loadShapesAction);
    }

    void addShape(const QPoint &position) {
        Shape *shape = nullptr;
        switch (currentShapeType) {
        case Shape::Rectangle: shape = new RectangleShape(position); break;
        case Shape::Triangle: shape = new TriangleShape(position); break;
        case Shape::Circle: shape = new CircleShape(position); break;
        }
        shapes.append(shape);
        update();
    }

    void changeShape() {
        currentShapeType = static_cast<Shape::Type>((currentShapeType + 1) % 3);
    }

    void saveShapes() {
        QFile file("shapes.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            for (const Shape *shape : shapes) {
                out << shape->saveToString() << "\n";
            }
            file.close();
        }
    }

    void loadShapes() {
        QFile file("shapes.txt");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                QStringList parts = line.split(' ');
                if (parts.size() == 3) {
                    int x = parts[1].toInt();
                    int y = parts[2].toInt();
                    if (parts[0] == "Rectangle") {
                        shapes.append(new RectangleShape(QPoint(x, y)));
                    } else if (parts[0] == "Triangle") {
                        shapes.append(new TriangleShape(QPoint(x, y)));
                    } else if (parts[0] == "Circle") {
                        shapes.append(new CircleShape(QPoint(x, y)));
                    }
                }
            }
            file.close();
            update();
        }
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        for (const Shape *shape : shapes) {
            shape->draw(painter);
        }
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            addShape(event->pos());
        } else if (event->button() == Qt::RightButton) {
            changeShape();
        }
    }

    void closeEvent(QCloseEvent *event) override {
        // Ask the user if they want to save the shapes before closing
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Save Changes", "Zelite li spremiti unesene promjene?",
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (reply == QMessageBox::Yes) {
            saveShapes();
            event->accept(); // Proceed with closing
        } else if (reply == QMessageBox::No) {
            event->accept(); // Proceed with closing without saving
        } else {
            event->ignore(); // Prevent closing
        }
    }

private:
    Shape::Type currentShapeType;
    QVector<Shape*> shapes;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ShapeDrawer window;
    window.show();

    return app.exec();
}

#include "main.moc"
