#pragma once

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class MainWindow : public QWidget {
	Q_OBJECT // MOC
public:
	explicit MainWindow(QWidget* parent = nullptr);

private slots:
	void onAddRectangle();
	void onAddEllipse();
	void onAddTriangle();
	void onDeleteSelected();

private:
	void setupUi();

private:
	QGraphicsScene* scene;
	QGraphicsView* view;
	QPushButton* addRectButton;
	QPushButton* addEllipseButton;
	QPushButton* addTriangleButton;
	QPushButton* deleteButton;
};