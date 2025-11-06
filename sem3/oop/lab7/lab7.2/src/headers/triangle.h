#pragma once

#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>
#include <QPolygonF>

class TriangleItem : public QGraphicsPolygonItem {
public:
	TriangleItem(qreal sideLength) {
		// Равносторонний треугольник, центр в (0,0)
		const qreal h = sideLength * std::sqrt(3.0) / 2.0;
		QPolygonF poly;
		poly << QPointF(0, -h / 2.0)
		     << QPointF(-sideLength / 2.0, h / 2.0)
		     << QPointF(sideLength / 2.0, h / 2.0);
		setPolygon(poly);
		setFlags(QGraphicsItem::ItemIsMovable |
		        QGraphicsItem::ItemIsSelectable |
		        QGraphicsItem::ItemSendsGeometryChanges |
		        QGraphicsItem::ItemIsFocusable);
	}

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
		setZValue(zValue() + 1000.0);
		QGraphicsPolygonItem::mousePressEvent(event);
	}
};

