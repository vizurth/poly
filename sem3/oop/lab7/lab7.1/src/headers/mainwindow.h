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

class MainWindow : public QWidget {
	Q_OBJECT // MOC
public:
	MainWindow(QWidget* parent = nullptr);

protected:
	void mousePressEvent(QMouseEvent* event) override; // посмотреть зачем?

private slots: // слоты для connect которые привязываем на кнопки
	void onAddRectangle();
	void onAddEllipse();
	void onAddTriangle();
	void onDeleteSelected();

private:
	void setupUi(); // реализовавываем UI
	void bringToFront(QWidget* widget); // поднять виджет на передний план

private:
	QWidget* canvas; // область для рисования фигур
	vector<QWidget*> shapes; // коллекция виджетов-фигур
	QWidget* selectedShape; // выделенная фигура

	// UI элементы
	QPushButton* addRectButton;
	QPushButton* addEllipseButton;
	QPushButton* addTriangleButton;
	QPushButton* deleteButton;
};
