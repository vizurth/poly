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
	explicit RectangleWidget(const QRectF& rect, const QColor& color, QWidget* parent = nullptr);
	void setSelected(bool selected);
	bool isSelected() const { return isSelected_; }

signals:
	void shapeSelected(RectangleWidget* widget);

protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

private:
	QColor color_;
	bool isSelected_;
	QPoint dragStartPosition_;
	bool isDragging_;
};
