#include "headers/mainwindow.h"
#include "headers/rectangle.h"
#include "headers/ellipse.h"
#include "headers/triangle.h"

#include <QRandomGenerator>
#include <QPolygonF>
#include <QScrollArea>
#include <algorithm>
#include <QMouseEvent>

static QColor randomColor() {
	return QColor::fromHsv(QRandomGenerator::global()->bounded(360), 180, 230);
}

MainWindow::MainWindow(QWidget* parent)
	: QWidget(parent), selectedShape(nullptr), pendingShapeType(ShapeType::None) {
	setupUi();
	setMinimumSize(820, 620);
}


void MainWindow::setupUi() {
	// создаем canvas - область для размещения фигур
	canvas = new QWidget(this);
	canvas->setMinimumSize(800, 600);
	canvas->setStyleSheet("background-color: white;");
	canvas->setMouseTracking(false);
	// перехватываем события мыши на canvas через eventFilter
	canvas->installEventFilter(this);

	// добавляем кнопки
	addRectButton = new QPushButton("Add Rectangle", this);
	addEllipseButton = new QPushButton("Add Ellipse", this);
	addTriangleButton = new QPushButton("Add Triangle", this);
	deleteButton = new QPushButton("Delete Selected", this);
	
	// делаем кнопки добавления checkable для визуального отображения активного состояния
	addRectButton->setCheckable(true);
	addEllipseButton->setCheckable(true);
	addTriangleButton->setCheckable(true);

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
	mainLayout->addWidget(canvas, 1); // добавляем canvas
	setLayout(mainLayout);
}

void MainWindow::bringToFront(QWidget* widget) { // поднимаем виджет наверх
	if (widget) {
		widget->raise();
	}
}

void MainWindow::resetAddButtons() {
	// сбрасываем подсветку всех кнопок добавления
	addRectButton->setChecked(false);
	addEllipseButton->setChecked(false);
	addTriangleButton->setChecked(false);
}

void MainWindow::updateShapesMouseTransparency() {
	// в режиме добавления фигур делаем существующие фигуры "прозрачными" для мыши,
	// чтобы клики проходили на холст и не вызывали выделение/перетаскивание
	const bool transparent = (pendingShapeType != ShapeType::None);
	for (QWidget* shape : shapes) {
		if (shape) {
			shape->setAttribute(Qt::WA_TransparentForMouseEvents, transparent);
		}
	}
}

void MainWindow::onAddRectangle() {
	// если уже в режиме добавления прямоугольника - отменяем режим
	if (pendingShapeType == ShapeType::Rectangle) {
		pendingShapeType = ShapeType::None;
		resetAddButtons();
		canvas->setCursor(Qt::ArrowCursor);
		updateShapesMouseTransparency();
		return;
	}
	// сбрасываем другие кнопки
	resetAddButtons();
	// устанавливаем режим ожидания клика для добавления прямоугольника
	pendingShapeType = ShapeType::Rectangle;
	addRectButton->setChecked(true); // подсвечиваем кнопку
	canvas->setCursor(Qt::CrossCursor); // меняем курсор на крестик
	updateShapesMouseTransparency();
}

void MainWindow::onAddEllipse() {
	// если уже в режиме добавления эллипса - отменяем режим
	if (pendingShapeType == ShapeType::Ellipse) {
		pendingShapeType = ShapeType::None;
		resetAddButtons();
		canvas->setCursor(Qt::ArrowCursor);
		updateShapesMouseTransparency();
		return;
	}
	// сбрасываем другие кнопки
	resetAddButtons();
	// устанавливаем режим ожидания клика для добавления эллипса
	pendingShapeType = ShapeType::Ellipse;
	addEllipseButton->setChecked(true); // подсвечиваем кнопку
	canvas->setCursor(Qt::CrossCursor); // меняем курсор на крестик
	updateShapesMouseTransparency();
}

void MainWindow::onAddTriangle() {
	// если уже в режиме добавления треугольника - отменяем режим
	if (pendingShapeType == ShapeType::Triangle) {
		pendingShapeType = ShapeType::None;
		resetAddButtons();
		canvas->setCursor(Qt::ArrowCursor);
		updateShapesMouseTransparency();
		return;
	}
	// сбрасываем другие кнопки
	resetAddButtons();
	// устанавливаем режим ожидания клика для добавления треугольника
	pendingShapeType = ShapeType::Triangle;
	addTriangleButton->setChecked(true); // подсвечиваем кнопку
	canvas->setCursor(Qt::CrossCursor); // меняем курсор на крестик
	updateShapesMouseTransparency();
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		QWidget* clickedWidget = childAt(event->pos());
		
		// если кликнули на кнопку или вне canvas в режиме добавления - отменяем режим
		if (pendingShapeType != ShapeType::None) {
			if (clickedWidget != canvas && 
			    clickedWidget != addRectButton && 
			    clickedWidget != addEllipseButton && 
			    clickedWidget != addTriangleButton && 
			    clickedWidget != deleteButton) {
				QPoint canvasPos = canvas->mapFromParent(event->pos());
				if (!canvas->rect().contains(canvasPos)) {
					// клик вне canvas отменяем режим добавления
					pendingShapeType = ShapeType::None;
					resetAddButtons(); // сбрасываем подсветку кнопок
					canvas->setCursor(Qt::ArrowCursor);
					updateShapesMouseTransparency();
				}
			}
		}
		
		// клик по canvas снимаем выделение
		if (clickedWidget == canvas || clickedWidget == nullptr) {
			selectedShape = nullptr;
			// снимаем выделение со всех фигур
			for (QWidget* shape : shapes) {
				if (auto* rect = qobject_cast<RectangleWidget*>(shape)) {
					rect->setSelected(false);
				} else if (auto* ellipse = qobject_cast<EllipseWidget*>(shape)) {
					ellipse->setSelected(false);
				} else if (auto* triangle = qobject_cast<TriangleWidget*>(shape)) {
					triangle->setSelected(false);
				}
			}
		}
	}
	QWidget::mousePressEvent(event);
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event) {
	// обрабатываем клики именно по canvas
	if (watched == canvas && event->type() == QEvent::MouseButtonPress) {
		auto* mouseEvent = static_cast<QMouseEvent*>(event);

		if (mouseEvent->button() == Qt::LeftButton) {
			const QPoint canvasPos = mouseEvent->pos(); // координаты относительно canvas

			// в режиме добавления фигур создаём фигуру только при клике по пустому месту
			if (pendingShapeType != ShapeType::None) {
				bool clickedOnShape = false;

				for (QWidget* shape : shapes) {
					if (!shape) continue;

					// geometry() фигуры в координатах родителя (canvas),поэтому можно напрямую проверять contains(canvasPos)
					if (shape->geometry().contains(canvasPos)) {
						clickedOnShape = true;
						break;
					}
				}

				if (!clickedOnShape) {
					createShapeAt(canvasPos, pendingShapeType);
				}

				// в любом случае не даём canvas обрабатывать событие дальше
				return true;
			}

			// не режим добавления — клик по холсту просто снимает выделение
			selectedShape = nullptr;
			for (QWidget* shape : shapes) {
				if (auto* rect = qobject_cast<RectangleWidget*>(shape)) {
					rect->setSelected(false);
				} else if (auto* ellipse = qobject_cast<EllipseWidget*>(shape)) {
					ellipse->setSelected(false);
				} else if (auto* triangle = qobject_cast<TriangleWidget*>(shape)) {
					triangle->setSelected(false);
				}
			}

			// считаем, что событие обработано
			return true;
		}
	}

	// остальные события передаём стандартной реализации
	return QWidget::eventFilter(watched, event);
}

void MainWindow::createShapeAt(const QPointF& position, ShapeType type) {
	QWidget* widget = nullptr;
	
	switch (type) {
		case ShapeType::Rectangle: {
			const QSizeF size(120, 80);
			// позиция центр фигуры
			QRectF rect(position.x() - size.width() / 2.0,
			           position.y() - size.height() / 2.0,
			           size.width(), size.height());
			widget = new RectangleWidget(rect, randomColor(), canvas);
			auto* rectWidget = qobject_cast<RectangleWidget*>(widget);
			connect(rectWidget, &RectangleWidget::shapeSelected, this, [this](RectangleWidget* w) {
				// снимаем выделение с других фигур
				for (QWidget* shape : shapes) {
					if (auto* rect = qobject_cast<RectangleWidget*>(shape)) {
						if (rect != w) rect->setSelected(false);
					} else if (auto* ellipse = qobject_cast<EllipseWidget*>(shape)) {
						ellipse->setSelected(false);
					} else if (auto* triangle = qobject_cast<TriangleWidget*>(shape)) {
						triangle->setSelected(false);
					}
				}
				selectedShape = w;
			});
			break;
		}
		case ShapeType::Ellipse: {
			const QSizeF size(120, 120);
			// позиция центр фигуры
			QRectF rect(position.x() - size.width() / 2.0,
			           position.y() - size.height() / 2.0,
			           size.width(), size.height());
			widget = new EllipseWidget(rect, randomColor(), canvas);
			auto* ellipseWidget = qobject_cast<EllipseWidget*>(widget);
			connect(ellipseWidget, &EllipseWidget::shapeSelected, this, [this](EllipseWidget* w) {
				// снимаем выделение с других фигур
				for (QWidget* shape : shapes) {
					if (auto* rect = qobject_cast<RectangleWidget*>(shape)) {
						rect->setSelected(false);
					} else if (auto* ellipse = qobject_cast<EllipseWidget*>(shape)) {
						if (ellipse != w) ellipse->setSelected(false);
					} else if (auto* triangle = qobject_cast<TriangleWidget*>(shape)) {
						triangle->setSelected(false);
					}
				}
				selectedShape = w;
			});
			break;
		}
		case ShapeType::Triangle: {
			const qreal side = 120.0;
			const qreal h = side * std::sqrt(3.0) / 2.0;
			// позиция центр треугольника (центр описанной окружности)
			QPolygonF poly;
			poly << QPointF(position.x(), position.y() - h * 2.0 / 3.0)  // верхняя вершина
			     << QPointF(position.x() - side / 2.0, position.y() + h / 3.0)  // левая нижняя
			     << QPointF(position.x() + side / 2.0, position.y() + h / 3.0); // правая нижняя
			widget = new TriangleWidget(poly, randomColor(), canvas);
			auto* triangleWidget = qobject_cast<TriangleWidget*>(widget);
			connect(triangleWidget, &TriangleWidget::shapeSelected, this, [this](TriangleWidget* w) {
				// снимаем выделение с других фигур
				for (QWidget* shape : shapes) {
					if (auto* rect = qobject_cast<RectangleWidget*>(shape)) {
						rect->setSelected(false);
					} else if (auto* ellipse = qobject_cast<EllipseWidget*>(shape)) {
						ellipse->setSelected(false);
					} else if (auto* triangle = qobject_cast<TriangleWidget*>(shape)) {
						if (triangle != w) triangle->setSelected(false);
					}
				}
				selectedShape = w;
			});
			break;
		}
		case ShapeType::None:
			return;
	}
	
	if (widget) {
		// Новая фигура должна подчиняться текущему режиму (кликабельна или "прозрачна" для мыши)
		const bool transparent = (pendingShapeType != ShapeType::None);
		widget->setAttribute(Qt::WA_TransparentForMouseEvents, transparent);

		widget->show();
		shapes.push_back(widget);
		bringToFront(widget);
	}
}

void MainWindow::onDeleteSelected() {
	// сбрасываем режим добавления при удалении
	if (pendingShapeType != ShapeType::None) {
		pendingShapeType = ShapeType::None;
		resetAddButtons(); // сбрасываем подсветку кнопок
		canvas->setCursor(Qt::ArrowCursor);
	}
	
	// удаляем выделенную фигуру или последнюю добавленную
	if (shapes.empty()) {
		return;
	}
	
	QWidget* toDelete = shapes.back();
	if (selectedShape) {
		// ищем выделенную фигуру
		auto it = std::find(shapes.begin(), shapes.end(), selectedShape);
		if (it != shapes.end()) {
			toDelete = *it;
		}
	}
	
	// удаляем из вектора
	shapes.erase(std::remove(shapes.begin(), shapes.end(), toDelete), shapes.end());
	delete toDelete;
	if (selectedShape == toDelete) {
		selectedShape = nullptr;
	}
}
