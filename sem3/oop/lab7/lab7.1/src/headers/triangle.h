#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QColor>
#include <QPolygonF>

// Виджет-треугольник, перегружает paintEvent и mouse events
// Для определения границ вписываем в прямоугольник
class TriangleWidget : public QWidget {
	Q_OBJECT
public:
	explicit TriangleWidget(const QPolygonF& polygon, const QColor& color, QWidget* parent = nullptr);
	void setSelected(bool selected);
	bool isSelected() const { return isSelected_; }

signals:
	void shapeSelected(TriangleWidget* widget);

protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

private:
	QPolygonF polygon_;
	QColor color_;
	bool isSelected_;
	QPoint dragStartPosition_;
	bool isDragging_;
};
