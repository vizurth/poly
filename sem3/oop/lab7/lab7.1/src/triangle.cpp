#include "headers/triangle.h"
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

TriangleWidget::TriangleWidget(const QPolygonF& polygon, const QColor& color, QWidget* parent)
    : QWidget(parent), polygon_(polygon), color(color), isSelected_(false), isDragging(false) {
    // вычисляем ограничивающий прямоугольник треугольника и добавляем отступ для контура
    QRectF boundingRect = polygon_.boundingRect();
    const int penPadding = 3; // максимальная ширина пера
    QRectF paddedRect = boundingRect.adjusted(-penPadding, -penPadding, penPadding, penPadding);
    setGeometry(paddedRect.toRect());

    // разрешаем обработку событий мыши
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
}

void TriangleWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(color);

    int penWidth = 2;
    painter.setPen(isSelected_ ? QPen(Qt::red, penWidth) : QPen(Qt::black, penWidth));

    QRectF boundingRect = polygon_.boundingRect();
    const int penPadding = 3;

    QPolygonF localPolygon = polygon_;
    for (int i = 0; i < localPolygon.size(); ++i) {
        localPolygon[i] = localPolygon[i] - boundingRect.topLeft() + QPointF(penPadding, penPadding);
    }

    painter.drawPolygon(localPolygon);
}

void TriangleWidget::setSelected(bool selected) {
    isSelected_ = selected;
    update();
}

// проверяем попадание точки в треугольник
bool TriangleWidget::pointInsideTriangle(const QPoint& p) const {
    QRectF boundingRect = polygon_.boundingRect();
    const int penPadding = 3;
    QPolygonF localPolygon = polygon_;
    for (int i = 0; i < localPolygon.size(); ++i) {
        localPolygon[i] = localPolygon[i] - boundingRect.topLeft() + QPointF(penPadding, penPadding);
    }
    return localPolygon.containsPoint(p, Qt::OddEvenFill);
}

void TriangleWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {

        // проверяем клик внутри треугольника
        if (!pointInsideTriangle(event->pos())) {
            event->ignore();
            return;
        }

        isDragging = true;
        dragStartPosition = event->pos();
        setSelected(true);
        raise();
        emit shapeSelected(this);
    }

    QWidget::mousePressEvent(event);
}

void TriangleWidget::mouseMoveEvent(QMouseEvent* event) {
    if (isDragging && (event->buttons() & Qt::LeftButton)) {
        QPoint delta = event->pos() - dragStartPosition;
        move(pos() + delta);
    }
    QWidget::mouseMoveEvent(event);
}

void TriangleWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
    }
    QWidget::mouseReleaseEvent(event);
}
