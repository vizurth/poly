#include "headers/triangle.h"
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

TriangleWidget::TriangleWidget(const QPolygonF& polygon, const QColor& color, QWidget* parent)
	: QWidget(parent), polygon_(polygon), color_(color), isSelected_(false), isDragging_(false) {
	// Устанавливаем геометрию виджета по ограничивающему прямоугольнику полигона
	// Добавляем отступ для контура (максимальная ширина пера = 3)
	QRectF boundingRect = polygon_.boundingRect();
	const int penPadding = 3; // максимальная ширина пера
	QRectF paddedRect = boundingRect.adjusted(-penPadding, -penPadding, penPadding, penPadding);
	setGeometry(paddedRect.toRect());
	setAttribute(Qt::WA_TransparentForMouseEvents, false);
}

void TriangleWidget::paintEvent(QPaintEvent* event) {
	Q_UNUSED(event);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	painter.setBrush(color_);
	int penWidth = isSelected_ ? 3 : 2;
	if (isSelected_) {
		painter.setPen(QPen(Qt::blue, penWidth));
	} else {
		painter.setPen(QPen(Qt::black, penWidth));
	}

	// Преобразуем полигон в локальные координаты виджета
	// Виджет увеличен на penPadding со всех сторон, поэтому треугольник нужно сместить
	QRectF boundingRect = polygon_.boundingRect();
	const int penPadding = 3; // должно совпадать с конструктором
	QPolygonF localPolygon = polygon_;
	// Позиция виджета = boundingRect.topLeft() - QPointF(penPadding, penPadding)
	// Поэтому для локальных координат: local = global - widgetPos = global - (boundingRect.topLeft() - QPointF(penPadding, penPadding))
	for (int i = 0; i < localPolygon.size(); ++i) {
		localPolygon[i] = localPolygon[i] - boundingRect.topLeft() + QPointF(penPadding, penPadding);
	}

	// Рисуем треугольник (контур уже не будет обрезаться, т.к. виджет увеличен)
	painter.drawPolygon(localPolygon);
}

void TriangleWidget::setSelected(bool selected) {
	isSelected_ = selected;
	update();
}

void TriangleWidget::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		isDragging_ = true;
		dragStartPosition_ = event->pos();
		setSelected(true);
		raise(); // поднимаем на передний план
		emit shapeSelected(this); // уведомляем MainWindow
	}
	QWidget::mousePressEvent(event);
}

void TriangleWidget::mouseMoveEvent(QMouseEvent* event) {
	if (isDragging_ && (event->buttons() & Qt::LeftButton)) {
		QPoint delta = event->pos() - dragStartPosition_;
		move(pos() + delta);
	}
	QWidget::mouseMoveEvent(event);
}

void TriangleWidget::mouseReleaseEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		isDragging_ = false;
	}
	QWidget::mouseReleaseEvent(event);
}

