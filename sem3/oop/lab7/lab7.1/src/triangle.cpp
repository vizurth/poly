#include "headers/triangle.h"
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

TriangleWidget::TriangleWidget(const QPolygonF& polygon, const QColor& color, QWidget* parent)
	: QWidget(parent), polygon_(polygon), color(color), isSelected_(false), isDragging(false) {
	// устанавливаем геометрию виджета по ограничивающему прямоугольнику полигона
	// добавляем отступ для контура (максимальная ширина пера = 3)
	QRectF boundingRect = polygon_.boundingRect();
	const int penPadding = 3; // максимальная ширина пера
	QRectF paddedRect = boundingRect.adjusted(-penPadding, -penPadding, penPadding, penPadding);
	setGeometry(paddedRect.toRect());
	setAttribute(Qt::WA_TransparentForMouseEvents, false);
}

void TriangleWidget::paintEvent(QPaintEvent* event) {
	Q_UNUSED(event); // макрос чтобы обойти warning
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing); // включаем сглаживание

	painter.setBrush(color); // задаём цвет заливки треугольника
	int penWidth = 2; // устанавливаем толщину обводки
	if (isSelected_) {
		painter.setPen(QPen(Qt::red, penWidth));
	} else {
		painter.setPen(QPen(Qt::black, penWidth));
	}

	// получаем прямоугольник который граница треугольника
	QRectF boundingRect = polygon_.boundingRect();
	const int penPadding = 3; // отступ от границ, чтобы контур не обрезался
	
	// рассчитываем новые точки виджета с учетом отступов
	QPolygonF localPolygon = polygon_; // создаём копию исходного полигона
	for (int i = 0; i < localPolygon.size(); ++i) {
		localPolygon[i] = localPolygon[i] - boundingRect.topLeft() + QPointF(penPadding, penPadding);
	}

	// рисуем сам треугольник
	painter.drawPolygon(localPolygon);
}

 
void TriangleWidget::setSelected(bool selected) {
	isSelected_ = selected;
	update();
}

void TriangleWidget::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		isDragging = true;
		dragStartPosition = event->pos();
		setSelected(true);
		raise(); // поднимаем на передний план
		emit shapeSelected(this); // уведомляем MainWindow
	}
	QWidget::mousePressEvent(event);
}

void TriangleWidget::mouseMoveEvent(QMouseEvent* event) {
	if (isDragging && (event->buttons() & Qt::LeftButton)) {
		QPoint delta = event->pos() - dragStartPosition;
		move(pos() + delta);
	}
	QWidget::mouseMoveEvent(event);
}

void TriangleWidget::mouseReleaseEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		isDragging = false;
	}
	QWidget::mouseReleaseEvent(event);
}

