#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QMouseEvent>
#include <vector>
#include <memory>

using namespace std;

enum class ShapeType {
	None,
	Rectangle,
	Ellipse,
	Triangle
};

class MainWindow : public QWidget {
	Q_OBJECT // MOC
public:
	MainWindow(QWidget* parent = nullptr);

protected:
	void mousePressEvent(QMouseEvent* event) override; // обработка кликов вне canvas
	bool eventFilter(QObject* watched, QEvent* event) override; // перехват событий на canvas

private slots: // слоты для connect которые привязываем на кнопки
	void onAddRectangle();
	void onAddEllipse();
	void onAddTriangle();
	void onDeleteSelected();

private:
	void setupUi(); // реализовавываем UI
	void bringToFront(QWidget* widget); // поднять виджет на передний план
	void createShapeAt(const QPointF& position, ShapeType type); // создать фигуру в указанной точке
	QWidget* createRectangleAt(const QPointF& position); // создать прямоугольник в указанной точке
	QWidget* createEllipseAt(const QPointF& position);   // создать эллипс в указанной точке
	QWidget* createTriangleAt(const QPointF& position);  // создать треугольник в указанной точке
	void resetAddButtons(); // сбросить подсветку всех кнопок добавления
	void updateShapesMouseTransparency(); // включить/выключить кликабельность фигур в зависимости от режима добавления

	void clearSelection(); // снимает выделение со всех фигур

private:
	QWidget* canvas; // область для рисования фигур
	vector<QWidget*> shapes; // коллекция виджетов-фигур
	QWidget* selectedShape; // выделенная фигура
	ShapeType pendingShapeType; // тип фигуры, ожидающей размещения

	// UI элементы
	QPushButton* addRectButton;
	QPushButton* addEllipseButton;
	QPushButton* addTriangleButton;
	QPushButton* deleteButton;
};
