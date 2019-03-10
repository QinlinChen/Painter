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
    p1.rx() = static_cast<int>(s * (p1.x() - x) + x);
    p1.ry() = static_cast<int>(s * (p1.y() - y) + y);
    p2.rx() = static_cast<int>(s * (p2.x() - x) + x);
    p2.ry() = static_cast<int>(s * (p2.y() - y) + y);
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
