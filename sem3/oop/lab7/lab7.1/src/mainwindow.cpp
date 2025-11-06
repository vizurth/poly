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
	: QWidget(parent), selectedShape_(nullptr) {
	setupUi();
	setMinimumSize(820, 620);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUi() {
	// Создаем canvas - область для размещения фигур
	canvas_ = new QWidget(this);
	canvas_->setMinimumSize(800, 600);
	canvas_->setStyleSheet("background-color: white;");
	canvas_->setMouseTracking(false);

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
	mainLayout->addWidget(canvas_, 1); // добавляем canvas
	setLayout(mainLayout);
}

void MainWindow::bringToFront(QWidget* widget) {
	if (widget) {
		widget->raise();
	}
}

void MainWindow::onAddRectangle() {
	const QSizeF size(120, 80);
	QRectF rect(canvas_->width() / 2.0 - size.width() / 2.0,
	           canvas_->height() / 2.0 - size.height() / 2.0,
	           size.width(), size.height());
	
	auto* widget = new RectangleWidget(rect, randomColor(), canvas_);
	connect(widget, &RectangleWidget::shapeSelected, this, [this](RectangleWidget* w) {
		// Снимаем выделение с других фигур
		for (QWidget* shape : shapes_) {
			if (auto* rect = qobject_cast<RectangleWidget*>(shape)) {
				if (rect != w) rect->setSelected(false);
			} else if (auto* ellipse = qobject_cast<EllipseWidget*>(shape)) {
				ellipse->setSelected(false);
			} else if (auto* triangle = qobject_cast<TriangleWidget*>(shape)) {
				triangle->setSelected(false);
			}
		}
		selectedShape_ = w;
	});
	widget->show();
	shapes_.push_back(widget);
	bringToFront(widget);
}

void MainWindow::onAddEllipse() {
	const QSizeF size(120, 120);
	QRectF rect(canvas_->width() / 2.0 - size.width() / 2.0,
	           canvas_->height() / 2.0 - size.height() / 2.0,
	           size.width(), size.height());
	
	auto* widget = new EllipseWidget(rect, randomColor(), canvas_);
	connect(widget, &EllipseWidget::shapeSelected, this, [this](EllipseWidget* w) {
		// Снимаем выделение с других фигур
		for (QWidget* shape : shapes_) {
			if (auto* rect = qobject_cast<RectangleWidget*>(shape)) {
				rect->setSelected(false);
			} else if (auto* ellipse = qobject_cast<EllipseWidget*>(shape)) {
				if (ellipse != w) ellipse->setSelected(false);
			} else if (auto* triangle = qobject_cast<TriangleWidget*>(shape)) {
				triangle->setSelected(false);
			}
		}
		selectedShape_ = w;
	});
	widget->show();
	shapes_.push_back(widget);
	bringToFront(widget);
}

void MainWindow::onAddTriangle() {
	const qreal side = 120.0;
	const qreal h = side * std::sqrt(3.0) / 2.0;
	QPolygonF poly;
	poly << QPointF(canvas_->width() / 2.0, canvas_->height() / 2.0 - h / 2.0)
	     << QPointF(canvas_->width() / 2.0 - side / 2.0, canvas_->height() / 2.0 + h / 2.0)
	     << QPointF(canvas_->width() / 2.0 + side / 2.0, canvas_->height() / 2.0 + h / 2.0);
	
	auto* widget = new TriangleWidget(poly, randomColor(), canvas_);
	connect(widget, &TriangleWidget::shapeSelected, this, [this](TriangleWidget* w) {
		// Снимаем выделение с других фигур
		for (QWidget* shape : shapes_) {
			if (auto* rect = qobject_cast<RectangleWidget*>(shape)) {
				rect->setSelected(false);
			} else if (auto* ellipse = qobject_cast<EllipseWidget*>(shape)) {
				ellipse->setSelected(false);
			} else if (auto* triangle = qobject_cast<TriangleWidget*>(shape)) {
				if (triangle != w) triangle->setSelected(false);
			}
		}
		selectedShape_ = w;
	});
	widget->show();
	shapes_.push_back(widget);
	bringToFront(widget);
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
	// Клик по canvas - снимаем выделение
	if (event->button() == Qt::LeftButton) {
		QWidget* clickedWidget = childAt(event->pos());
		// Проверяем, не кликнули ли мы по canvas или по кнопкам
		if (clickedWidget == canvas_ || clickedWidget == nullptr) {
			selectedShape_ = nullptr;
			// Снимаем выделение со всех фигур
			for (QWidget* shape : shapes_) {
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
	if (shapes_.empty()) {
		return;
	}
	
	QWidget* toDelete = shapes_.back();
	if (selectedShape_) {
		// Ищем выделенную фигуру
		auto it = std::find(shapes_.begin(), shapes_.end(), selectedShape_);
		if (it != shapes_.end()) {
			toDelete = *it;
		}
	}
	
	// Удаляем из вектора
	shapes_.erase(std::remove(shapes_.begin(), shapes_.end(), toDelete), shapes_.end());
	delete toDelete;
	if (selectedShape_ == toDelete) {
		selectedShape_ = nullptr;
	}
}
