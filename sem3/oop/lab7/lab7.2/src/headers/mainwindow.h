#pragma once

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>

enum class ShapeType {
	None,
	Rectangle,
	Ellipse,
	Triangle,
	Star
};

// Forward declaration
class MainWindow;

class CustomGraphicsScene : public QGraphicsScene {
	Q_OBJECT
public:
	explicit CustomGraphicsScene(QObject* parent = nullptr) : QGraphicsScene(parent) {}
	
	void setMainWindow(MainWindow* mw);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
	MainWindow* mainWindow = nullptr;
};

class MainWindow : public QWidget {
	Q_OBJECT // MOC
public:
	explicit MainWindow(QWidget* parent = nullptr);


private slots:
	void onAddRectangle();
	void onAddEllipse();
	void onAddTriangle();
	void onAddStar();
	void onDeleteSelected();

private:
	void setupUi();
	void createShapeAt(const QPointF& scenePos, ShapeType type); // создать фигуру в указанной точке сцены
	void resetAddButtons(); // сбросить подсветку всех кнопок добавления
	void setItemsSelectable(bool enabled); // включить/выключить возможность выделения фигур

	friend class CustomGraphicsScene; // разрешаем CustomGraphicsScene доступ к приватным членам
	
private:
	CustomGraphicsScene* scene;
	QGraphicsView* view;
	ShapeType pendingShapeType; // тип фигуры, ожидающей размещения
	
	QPushButton* addRectButton;
	QPushButton* addEllipseButton;
	QPushButton* addTriangleButton;
	QPushButton* addStarButton;
	QPushButton* deleteButton;
};