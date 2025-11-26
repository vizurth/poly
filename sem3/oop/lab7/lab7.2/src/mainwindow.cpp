#include "headers/mainwindow.h"

#include <QRandomGenerator>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QPainterPath>
#include <QGraphicsSceneMouseEvent>

#include "headers/rectangle.h"
#include "headers/ellipse.h"
#include "headers/triangle.h"
#include "headers/star.h"

namespace {
	static qreal g_nextZ = 1.0;
	static QPointF sceneCenter(QGraphicsScene* scene) {
		QRectF r = scene->sceneRect();
		return r.center();
	}
}

MainWindow::MainWindow(QWidget* parent)
	: QWidget(parent), pendingShapeType(ShapeType::None) {
	setupUi();
}

void MainWindow::setupUi() {
	scene = new CustomGraphicsScene(this); // контейнер для виджетов
	scene->setSceneRect(0, 0, 800, 600);
	scene->setMainWindow(this); // передаем указатель на MainWindow

	view = new QGraphicsView(scene, this); // виджет который отображает содержимое scene
	view->setDragMode(QGraphicsView::RubberBandDrag); // используется для когда зажимаем обьект
	view->setMinimumSize(820, 620);
	view->setMouseTracking(false);

	// добавляем кнопки
	addRectButton = new QPushButton("Add Rectangle", this);
	addEllipseButton = new QPushButton("Add Ellipse", this);
	addTriangleButton = new QPushButton("Add Triangle", this);
	addStarButton = new QPushButton("Add Star", this);
	deleteButton = new QPushButton("Delete Selected", this);
	
	// Делаем кнопки добавления checkable для визуального отображения активного состояния
	addRectButton->setCheckable(true);
	addEllipseButton->setCheckable(true);
	addTriangleButton->setCheckable(true);
	addStarButton->setCheckable(true);

	// подключаем коннекты функция к кнопке
	connect(addRectButton, &QPushButton::clicked, this, &MainWindow::onAddRectangle);
	connect(addEllipseButton, &QPushButton::clicked, this, &MainWindow::onAddEllipse);
	connect(addTriangleButton, &QPushButton::clicked, this, &MainWindow::onAddTriangle);
	connect(addStarButton, &QPushButton::clicked, this, &MainWindow::onAddStar);
	connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteSelected);

	// создаем горизонтальный layout
	auto* buttonsLayout = new QHBoxLayout();
	buttonsLayout->addWidget(addRectButton);
	buttonsLayout->addWidget(addEllipseButton);
	buttonsLayout->addWidget(addTriangleButton);
	buttonsLayout->addWidget(addStarButton);
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

void MainWindow::resetAddButtons() {
	// Сбрасываем подсветку всех кнопок добавления
	addRectButton->setChecked(false);
	addEllipseButton->setChecked(false);
	addTriangleButton->setChecked(false);
	addStarButton->setChecked(false);
}

void MainWindow::setItemsSelectable(bool enabled) {
	if (!scene) return;
	for (QGraphicsItem* item : scene->items()) {
		item->setFlag(QGraphicsItem::ItemIsSelectable, enabled);
	}
}

void MainWindow::onAddRectangle() {
	// Если уже в режиме добавления прямоугольника - отменяем режим
	if (pendingShapeType == ShapeType::Rectangle) {
		pendingShapeType = ShapeType::None;
		resetAddButtons();
		setItemsSelectable(true); // возвращаем возможность выделения фигур
		view->setCursor(Qt::ArrowCursor);
		view->setDragMode(QGraphicsView::RubberBandDrag); // возвращаем режим выделения
		return;
	}
	// Сбрасываем другие кнопки
	resetAddButtons();
	// Устанавливаем режим ожидания клика для добавления прямоугольника
	pendingShapeType = ShapeType::Rectangle;
	addRectButton->setChecked(true); // подсвечиваем кнопку
	setItemsSelectable(false); // запрещаем выделение фигур в режиме добавления
	view->setCursor(Qt::CrossCursor); // меняем курсор на крестик
	view->setDragMode(QGraphicsView::NoDrag); // отключаем выделение
}

void MainWindow::onAddEllipse() {
	// Если уже в режиме добавления эллипса - отменяем режим
	if (pendingShapeType == ShapeType::Ellipse) {
		pendingShapeType = ShapeType::None;
		resetAddButtons();
		setItemsSelectable(true); // возвращаем возможность выделения фигур
		view->setCursor(Qt::ArrowCursor);
		view->setDragMode(QGraphicsView::RubberBandDrag); // возвращаем режим выделения
		return;
	}
	// Сбрасываем другие кнопки
	resetAddButtons();
	// Устанавливаем режим ожидания клика для добавления эллипса
	pendingShapeType = ShapeType::Ellipse;
	addEllipseButton->setChecked(true); // подсвечиваем кнопку
	setItemsSelectable(false); // запрещаем выделение фигур в режиме добавления
	view->setCursor(Qt::CrossCursor); // меняем курсор на крестик
	view->setDragMode(QGraphicsView::NoDrag); // отключаем выделение
}

void MainWindow::onAddTriangle() {
	// Если уже в режиме добавления треугольника - отменяем режим
	if (pendingShapeType == ShapeType::Triangle) {
		pendingShapeType = ShapeType::None;
		resetAddButtons();
		setItemsSelectable(true); // возвращаем возможность выделения фигур
		view->setCursor(Qt::ArrowCursor);
		view->setDragMode(QGraphicsView::RubberBandDrag); // возвращаем режим выделения
		return;
	}
	// Сбрасываем другие кнопки
	resetAddButtons();
	// Устанавливаем режим ожидания клика для добавления треугольника
	pendingShapeType = ShapeType::Triangle;
	addTriangleButton->setChecked(true); // подсвечиваем кнопку
	setItemsSelectable(false); // запрещаем выделение фигур в режиме добавления
	view->setCursor(Qt::CrossCursor); // меняем курсор на крестик
	view->setDragMode(QGraphicsView::NoDrag); // отключаем выделение
}

void MainWindow::onAddStar() {
	// Если уже в режиме добавления звезды - отменяем режим
	if (pendingShapeType == ShapeType::Star) {
		pendingShapeType = ShapeType::None;
		resetAddButtons();
		setItemsSelectable(true); // возвращаем возможность выделения фигур
		view->setCursor(Qt::ArrowCursor);
		view->setDragMode(QGraphicsView::RubberBandDrag); // возвращаем режим выделения
		return;
	}
	// Сбрасываем другие кнопки
	resetAddButtons();
	// Устанавливаем режим ожидания клика для добавления звезды
	pendingShapeType = ShapeType::Star;
	addStarButton->setChecked(true); // подсвечиваем кнопку
	setItemsSelectable(false); // запрещаем выделение фигур в режиме добавления
	view->setCursor(Qt::CrossCursor); // меняем курсор на крестик
	view->setDragMode(QGraphicsView::NoDrag); // отключаем выделение
}

void CustomGraphicsScene::setMainWindow(MainWindow* mw) {
	mainWindow = mw;
}

void CustomGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	if (mainWindow && event->button() == Qt::LeftButton) {
		// Если мы в режиме добавления фигуры
		if (mainWindow->pendingShapeType != ShapeType::None) {
			QPointF scenePos = event->scenePos();
			
			// Проверяем, что клик не на существующей фигуре
			QGraphicsItem* itemAtPos = itemAt(scenePos, QTransform());
			if (!itemAtPos) {
				// Клик на пустом месте - создаем фигуру
				mainWindow->createShapeAt(scenePos, mainWindow->pendingShapeType);
			}
			// В любом случае принимаем событие, чтобы фигуры не выделялись
			event->accept();
			return;
		}
	}
	// Для всех остальных случаев передаем обработку базовому классу
	QGraphicsScene::mousePressEvent(event);
}

void MainWindow::createShapeAt(const QPointF& scenePos, ShapeType type) {
	QGraphicsItem* item = nullptr;
	
	switch (type) {
		case ShapeType::Rectangle: {
			const QSizeF size(120, 80);
			QRectF rect(QPointF(-size.width() / 2.0, -size.height() / 2.0), size);
			RectangleItem* rectItem = new RectangleItem(rect);
			rectItem->setBrush(randomColor()); // цвет заливки
			rectItem->setPen(QPen(Qt::black, 2)); // контур
			rectItem->setPos(scenePos); // задаем позицию на сцене в месте клика
			rectItem->setZValue(g_nextZ++); // увеличиваем zCount
			scene->addItem(rectItem); // добавляем на сцену
			item = rectItem;
			break;
		}
		case ShapeType::Ellipse: {
			const QSizeF size(120, 120);
			QRectF rect(QPointF(-size.width() / 2.0, -size.height() / 2.0), size);
			EllipseItem* ellipseItem = new EllipseItem(rect);
			ellipseItem->setBrush(randomColor()); // цвет заливки
			ellipseItem->setPen(QPen(Qt::black, 2)); // контур
			ellipseItem->setPos(scenePos); // задаем позицию на сцене в месте клика
			ellipseItem->setZValue(g_nextZ++); // увеличиваем zCount
			scene->addItem(ellipseItem); // добавляем на сцену
			item = ellipseItem;
			break;
		}
		case ShapeType::Triangle: {
			const qreal side = 120.0;
			TriangleItem* triangleItem = new TriangleItem(side);
			triangleItem->setBrush(randomColor());
			triangleItem->setPen(QPen(Qt::black, 2));
			triangleItem->setPos(scenePos); // задаем позицию на сцене в месте клика
			triangleItem->setZValue(g_nextZ++);
			scene->addItem(triangleItem);
			item = triangleItem;
			break;
		}
		case ShapeType::Star: {
			const qreal outerRadius = 60.0;
			const qreal innerRadius = 25.0;
			StarItem* starItem = new StarItem(outerRadius, innerRadius, 5);
			starItem->setBrush(randomColor());
			starItem->setPen(QPen(Qt::black, 2));
			starItem->setPos(scenePos); // задаем позицию на сцене в месте клика
			starItem->setZValue(g_nextZ++);
			scene->addItem(starItem);
			item = starItem;
			break;
		}
		case ShapeType::None:
			return;
	}

	// Если мы находимся в режиме добавления фигуры, делаем только что созданный элемент
	// тоже невыделяемым, чтобы клики по нему не приводили к выделению
	if (item && pendingShapeType != ShapeType::None) {
		item->setFlag(QGraphicsItem::ItemIsSelectable, false);
	}
}

void MainWindow::onDeleteSelected() {
	// Сбрасываем режим добавления при удалении
	if (pendingShapeType != ShapeType::None) {
		pendingShapeType = ShapeType::None;
		resetAddButtons(); // сбрасываем подсветку кнопок
		setItemsSelectable(true); // возвращаем возможность выделения фигур
		view->setCursor(Qt::ArrowCursor);
		view->setDragMode(QGraphicsView::RubberBandDrag); // возвращаем режим выделения
	}
	
	const auto selected = scene->selectedItems();
	for (QGraphicsItem* it : selected) {
		delete it; // удаление из сцены и памяти
	}
}