#include "line.h"

#include <QPainter>

Line::Line(const QPoint &point1, const QPoint &point2,
           const QColor &color, const QString &algorithm)
    : p1(point1), p2(point2), deltaCenter(0, 0), c(color), alg(algorithm)
{

}

QString Line::shapeName()
{
    return "Line";
}

void Line::draw(QImage &canvas)
{
    QPainter painter(&canvas);
    painter.setPen(c);
    painter.drawLine(p1, p2);
}

void Line::translate(int dx, int dy)
{
    QPoint delta(dx, dy);
    p1 += delta;
    p2 += delta;
}

void Line::rotate(int x, int y, float r)
{
    // TODO
}

void Line::scale(int x, int y, float s)
{
    // TODO
}

QRect Line::getRectHull()
{
    return QRect(p1, p2).normalized();
}

QPoint Line::getCenter()
{
    return getRectHull().center() + deltaCenter;
}

void Line::moveCenter(int dx, int dy)
{
    deltaCenter += QPoint(dx, dy);
}
