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
	: QWidget(parent), selectedShape(nullptr) {
	setupUi();
	setMinimumSize(820, 620);
}


void MainWindow::setupUi() {
	// Создаем canvas - область для размещения фигур
	canvas = new QWidget(this);
	canvas->setMinimumSize(800, 600);
	canvas->setStyleSheet("background-color: white;");
	canvas->setMouseTracking(false);

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
	mainLayout->addWidget(canvas, 1); // добавляем canvas
	setLayout(mainLayout);
}

void MainWindow::bringToFront(QWidget* widget) { // поднимаем виджет наверх
	if (widget) {
		widget->raise();
	}
}

void MainWindow::onAddRectangle() {
	const QSizeF size(120, 80); // задаем размер
	QRectF rect(canvas->width() / 2.0 - size.width() / 2.0,
	           canvas->height() / 2.0 - size.height() / 2.0,
	           size.width(), size.height()); // создаем rect через конструктор
	
	auto* widget = new RectangleWidget(rect, randomColor(), canvas);
	connect(widget, &RectangleWidget::shapeSelected, this, [this](RectangleWidget* w) {
		// Снимаем выделение с других фигур
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
	widget->show();
	shapes.push_back(widget);
	bringToFront(widget);
}

void MainWindow::onAddEllipse() {
	const QSizeF size(120, 120);
	QRectF rect(canvas->width() / 2.0 - size.width() / 2.0,
	           canvas->height() / 2.0 - size.height() / 2.0,
	           size.width(), size.height());
	
	auto* widget = new EllipseWidget(rect, randomColor(), canvas);
	connect(widget, &EllipseWidget::shapeSelected, this, [this](EllipseWidget* w) {
		// Снимаем выделение с других фигур
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
	widget->show();
	shapes.push_back(widget);
	bringToFront(widget);
}

void MainWindow::onAddTriangle() {
	const qreal side = 120.0;
	const qreal h = side * std::sqrt(3.0) / 2.0;
	QPolygonF poly;
	poly << QPointF(canvas->width() / 2.0, canvas->height() / 2.0 - h / 2.0)
	     << QPointF(canvas->width() / 2.0 - side / 2.0, canvas->height() / 2.0 + h / 2.0)
	     << QPointF(canvas->width() / 2.0 + side / 2.0, canvas->height() / 2.0 + h / 2.0);
	
	auto* widget = new TriangleWidget(poly, randomColor(), canvas);
	connect(widget, &TriangleWidget::shapeSelected, this, [this](TriangleWidget* w) {
		// Снимаем выделение с других фигур
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
	widget->show();
	shapes.push_back(widget);
	bringToFront(widget);
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
	// Клик по canvas - снимаем выделение
	if (event->button() == Qt::LeftButton) {
		QWidget* clickedWidget = childAt(event->pos());
		// Проверяем, не кликнули ли мы по canvas или по кнопкам
		if (clickedWidget == canvas || clickedWidget == nullptr) {
			selectedShape = nullptr;
			// Снимаем выделение со всех фигур
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

void MainWindow::onDeleteSelected() {
	// Удаляем выделенную фигуру или последнюю добавленную
	if (shapes.empty()) {
		return;
	}
	
	QWidget* toDelete = shapes.back();
	if (selectedShape) {
		// Ищем выделенную фигуру
		auto it = std::find(shapes.begin(), shapes.end(), selectedShape);
		if (it != shapes.end()) {
			toDelete = *it;
		}
	}
	
	// Удаляем из вектора
	shapes.erase(std::remove(shapes.begin(), shapes.end(), toDelete), shapes.end());
	delete toDelete;
	if (selectedShape == toDelete) {
		selectedShape = nullptr;
	}
}
