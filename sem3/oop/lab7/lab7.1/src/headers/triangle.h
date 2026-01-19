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
	explicit TriangleWidget(const QPolygonF& polygon, const QColor& color, QWidget* parent = nullptr); // конструктор
	void setSelected(bool selected); // функуция для выделения
	bool isSelected() const { return isSelected_; } // проверка на выделение

signals:
	void shapeSelected(TriangleWidget* widget);

protected:
	void paintEvent(QPaintEvent* event) override; // перегрузка методов из quest.md
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	bool pointInsideTriangle(const QPoint& p) const;

private:
	QPolygonF polygon_;
	QColor color;
	bool isSelected_;
	QPoint dragStartPosition;
	bool isDragging;
};
