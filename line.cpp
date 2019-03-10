#include "line.h"
#include "utils.h"

#include <QPainter>

Line::Line(const QPoint &point1, const QPoint &point2,
           const QColor &color, const QString &algorithm)
    : p1(point1), p2(point2), c(color), alg(algorithm), autoCenter(true)
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
    return autoCenter ? getRectHull().center() : center;
}

void Line::setCenter(int x, int y)
{
    center.setX(x);
    center.setY(y);
    autoCenter = Utils::isClose(getRectHull().center(), center, 4);
}
