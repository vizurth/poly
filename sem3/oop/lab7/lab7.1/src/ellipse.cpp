#include "headers/ellipse.h"
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

EllipseWidget::EllipseWidget(const QRectF& rect, const QColor& color, QWidget* parent)
	: QWidget(parent), color(color), isSelected_(false), isDragging(false) {
	setGeometry(rect.toRect()); // задаём положение и размер виджета по переданному прямоугольнику 
	setAttribute(Qt::WA_TransparentForMouseEvents, false); // разрешаем взаимодействие с мышкой
}

void EllipseWidget::paintEvent(QPaintEvent* event) {
	Q_UNUSED(event); // макрос чтобы избежать warning
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing); // сглаживание

	painter.setBrush(color); // заливка
	int penWidth = 2; // выделение при селекте
	if (isSelected_) {
		painter.setPen(QPen(Qt::red, penWidth));
	} else {
		painter.setPen(QPen(Qt::black, penWidth));
	}

	// рисуем эллипс с отступом от краев, чтобы контур не обрезался
	// отступ равен половине ширины пера
	qreal penHalf = penWidth / 2.0;
	QRectF drawRect(penHalf, penHalf, 
	                width() - penWidth, 
	                height() - penWidth);
	painter.drawEllipse(drawRect);
}

void EllipseWidget::setSelected(bool selected) {
	isSelected_ = selected;
	update(); // перерисовка чтобы появилась синяя граница
}

void EllipseWidget::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		isDragging = true;
		dragStartPosition = event->pos();
		setSelected(true);
		raise(); // поднимаем на передний план
		emit shapeSelected(this); // уведомляем MainWindow emit макрос для отправки сигналов
	}
	QWidget::mousePressEvent(event);
}

void EllipseWidget::mouseMoveEvent(QMouseEvent* event) {
	if (isDragging && (event->buttons() & Qt::LeftButton)) {
		QPoint delta = event->pos() - dragStartPosition; // сдвиг корсора
		move(pos() + delta);
	}
	QWidget::mouseMoveEvent(event); // проводим имент
}

void EllipseWidget::mouseReleaseEvent(QMouseEvent* event) { // отпускаем кнопку
	if (event->button() == Qt::LeftButton) {
		isDragging = false;
	}
	QWidget::mouseReleaseEvent(event);
}

