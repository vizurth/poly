#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QColor>
#include <QRectF>

// виджет-эллипс, перегружает paintEvent и mouse events
// Для определения границ вписываем в прямоугольник
class EllipseWidget : public QWidget {
	Q_OBJECT
public:
	EllipseWidget(const QRectF& rect, const QColor& color, QWidget* parent = nullptr); // конструктор
	void setSelected(bool selected); // функция для выделения
	bool isSelected() const { return isSelected_; } // проверка на выделение

signals:
	void shapeSelected(EllipseWidget* widget);

protected:
	void paintEvent(QPaintEvent* event) override; // перегрузка методов из quest.md
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

private:
	QColor color;
	bool isSelected_;
	QPoint dragStartPosition;
	bool isDragging;
};
