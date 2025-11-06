#include "headers/mainwindow.h"

#include <QRandomGenerator>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QPainterPath>

#include "headers/rectangle.h"
#include "headers/ellipse.h"
#include "headers/triangle.h"

namespace {
	static qreal g_nextZ = 1.0;
	static QPointF sceneCenter(QGraphicsScene* scene) {
		QRectF r = scene->sceneRect();
		return r.center();
	}
}

MainWindow::MainWindow(QWidget* parent)
	: QWidget(parent) {
	setupUi();
}

void MainWindow::setupUi() {
	scene = new QGraphicsScene(this); // контейнер для виджетов
	scene->setSceneRect(0, 0, 800, 600);

	view = new QGraphicsView(scene, this); // виджет который отображает содержимое scene
	view->setDragMode(QGraphicsView::RubberBandDrag); // используется для когда зажимаем обьект
	view->setMinimumSize(820, 620);

	// добавляем кнопки
	addRectButton = new QPushButton("Add Rectangle", this);
	addEllipseButton = new QPushButton("Add Ellipse", this);
	addTriangleButton = new QPushButton("Add Triangle", this);
	deleteButton = new QPushButton("Delete Selected", this);

	// подключаем коннекты функция к кнопке
	connect(addRectButton, &QPushButton::clicked, this, &MainWindow::onAddRectangle);
	connect(addEllipseButton, &QPushButton::clicked, this, &MainWindow::onAddEllipse);
	connect(addTriangleButton, &QPushButton::clicked, this, &MainWindow::onAddTriangle);
	connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteSelected);

	// создаем горизонтальный layout
	auto* buttonsLayout = new QHBoxLayout();
	buttonsLayout->addWidget(addRectButton);
	buttonsLayout->addWidget(addEllipseButton);
	buttonsLayout->addWidget(addTriangleButton);
	buttonsLayout->addWidget(deleteButton);
	buttonsLayout->addStretch(1);

	// создаем вертикальный mainLayout
	auto* mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(buttonsLayout); // добавляем кнопки
	mainLayout->addWidget(view, 1); // добавляем GraphicsView
	setLayout(mainLayout);
}

static QColor randomColor() {
	return QColor::fromHsv(QRandomGenerator::global()->bounded(360), 180, 230);
}

void MainWindow::onAddRectangle() {
	const QSizeF size(120, 80);
	QRectF rect(QPointF(-size.width() / 2.0, -size.height() / 2.0), size);
	auto* item = new RectangleItem(rect);
	item->setBrush(randomColor()); // цвет заливки
	item->setPen(QPen(Qt::black, 2)); // контур
	item->setPos(sceneCenter(scene)); // задаем позицию на view
	item->setZValue(g_nextZ++); // увеличиваем zCount
	scene->addItem(item); // добавляем на сцену
}

void MainWindow::onAddEllipse() {
	const QSizeF size(120, 120);
	QRectF rect(QPointF(-size.width() / 2.0, -size.height() / 2.0), size);
	auto* item = new EllipseItem(rect);
	item->setBrush(randomColor()); // цвет заливки
	item->setPen(QPen(Qt::black, 2)); // контур
	item->setPos(sceneCenter(scene)); // задаем позицию на view
	item->setZValue(g_nextZ++); // увеличиваем zCount
	scene->addItem(item); // добавляем на сцену
}

void MainWindow::onAddTriangle() {
	const qreal side = 120.0;
	auto* item = new TriangleItem(side);
	item->setBrush(randomColor());
	item->setPen(QPen(Qt::black, 2));
	item->setPos(sceneCenter(scene));
	item->setZValue(g_nextZ++);
	scene->addItem(item);
}

void MainWindow::onDeleteSelected() {
	const auto selected = scene->selectedItems();
	for (QGraphicsItem* it : selected) {
		delete it; // удаление из сцены и памяти
	}
}

