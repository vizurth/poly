#pragma once

#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>
#include <QPolygonF>
#include <cmath>

class StarItem : public QGraphicsPolygonItem {
public:
	StarItem(qreal outerRadius, qreal innerRadius, int points = 5) {
		// Создаем звезду с заданным количеством лучей
		QPolygonF poly;
		const qreal angleStep = M_PI / points; // угол между вершинами
		
		for (int i = 0; i < points * 2; ++i) {
			qreal radius = (i % 2 == 0) ? outerRadius : innerRadius;
			qreal angle = i * angleStep - M_PI / 2.0; // начинаем сверху
			qreal x = radius * std::cos(angle);
			qreal y = radius * std::sin(angle);
			poly << QPointF(x, y);
		}
		
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