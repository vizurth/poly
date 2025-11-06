#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QMouseEvent>
#include <vector>
#include <memory>

class MainWindow : public QWidget {
	Q_OBJECT // MOC
public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

protected:
	void mousePressEvent(QMouseEvent* event) override;

private slots:
	void onAddRectangle();
	void onAddEllipse();
	void onAddTriangle();
	void onDeleteSelected();

private:
	void setupUi();
	void bringToFront(QWidget* widget); // поднять виджет на передний план

private:
	QWidget* canvas_; // область для рисования фигур
	std::vector<QWidget*> shapes_; // коллекция виджетов-фигур
	QWidget* selectedShape_; // выделенная фигура

	// UI элементы
	QPushButton* addRectButton;
	QPushButton* addEllipseButton;
	QPushButton* addTriangleButton;
	QPushButton* deleteButton;
};
