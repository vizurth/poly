#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QColor>
#include <QRectF>

// Виджет-прямоугольник, перегружает paintEvent и mouse events
class RectangleWidget : public QWidget {
	Q_OBJECT
public:
	RectangleWidget(const QRectF& rect, const QColor& color, QWidget* parent = nullptr); // конструктор
	void setSelected(bool selected); // функция для выделения
	bool isSelected() const { return isSelected_; } // проверка на выделение

signals:
	void shapeSelected(RectangleWidget* widget);

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
