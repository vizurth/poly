#pragma once

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

class RectangleItem : public QGraphicsRectItem {
public:
	RectangleItem(const QRectF& rect)
		: QGraphicsRectItem(rect) {
		setFlags(QGraphicsItem::ItemIsMovable |
		        QGraphicsItem::ItemIsSelectable |
		        QGraphicsItem::ItemSendsGeometryChanges |
		        QGraphicsItem::ItemIsFocusable);
	}

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
		setZValue(zValue() + 1000.0); // поднять на передний план
		QGraphicsRectItem::mousePressEvent(event);
	}
};

