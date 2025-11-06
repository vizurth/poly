#include "headers/rectangle.h"
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

RectangleWidget::RectangleWidget(const QRectF& rect, const QColor& color, QWidget* parent)
	: QWidget(parent), color_(color), isSelected_(false), isDragging_(false) {
	// Устанавливаем геометрию виджета по ограничивающему прямоугольнику
	setGeometry(rect.toRect());
	setAttribute(Qt::WA_TransparentForMouseEvents, false); // виджет должен получать события мыши
}

void RectangleWidget::paintEvent(QPaintEvent* event) {
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

	// Рисуем прямоугольник с отступом от краев, чтобы контур не обрезался
	// Отступ равен половине ширины пера
	qreal penHalf = penWidth / 2.0;
	QRectF drawRect(penHalf, penHalf, 
	                width() - penWidth, 
	                height() - penWidth);
	painter.drawRect(drawRect);
}

void RectangleWidget::setSelected(bool selected) {
	isSelected_ = selected;
	update();
}

void RectangleWidget::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		isDragging_ = true;
		dragStartPosition_ = event->pos();
		setSelected(true);
		raise(); // поднимаем на передний план
		emit shapeSelected(this); // уведомляем MainWindow
	}
	QWidget::mousePressEvent(event);
}

void RectangleWidget::mouseMoveEvent(QMouseEvent* event) {
	if (isDragging_ && (event->buttons() & Qt::LeftButton)) {
		QPoint delta = event->pos() - dragStartPosition_;
		move(pos() + delta);
	}
	QWidget::mouseMoveEvent(event);
}

void RectangleWidget::mouseReleaseEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		isDragging_ = false;
	}
	QWidget::mouseReleaseEvent(event);
}

