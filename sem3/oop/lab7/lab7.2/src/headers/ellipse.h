#pragma once
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>

class EllipseItem : public QGraphicsEllipseItem {
public:
	EllipseItem(const QRectF& rect)
		: QGraphicsEllipseItem(rect) {
		setFlags(QGraphicsItem::ItemIsMovable |
		        QGraphicsItem::ItemIsSelectable |
		        QGraphicsItem::ItemSendsGeometryChanges |
		        QGraphicsItem::ItemIsFocusable);
	}

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
		setZValue(zValue() + 1000.0);
		QGraphicsEllipseItem::mousePressEvent(event);
	}
};