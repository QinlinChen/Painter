#include "line.h"
#include "utils.h"

#include <QPainter>

Line::Line(const QPoint &point1, const QPoint &point2,
           const QColor &color, const QString &algorithm)
    : p1(point1), p2(point2), c(color), alg(algorithm)
{

}

QString Line::shapeName()
{
    return "Line";
}

void Line::beginTransaction()
{
    oldp1 = p1;
    oldp2 = p2;
    Shape::beginTransaction();
}

void Line::commitTransaction()
{
    Shape::commitTransaction();
}

void Line::rollbackTransaction()
{
    p1 = oldp1;
    p2 = oldp2;
    Shape::rollbackTransaction();
}

void Line::draw(QImage &canvas)
{
    QPainter painter(&canvas);
    painter.setPen(c);
    painter.drawLine(p1, p2);
}

void Line::translate(const QPoint &d)
{
    if (duringTransaction) {
        p1 = oldp1 + d;
        p2 = oldp2 + d;
    }
    else {
        p1 += d;
        p2 += d;
    }
}

void Line::rotate(const QPoint &c, float r)
{
    // TODO
}

void Line::scale(const QPoint &c, float s)
{
    if (duringTransaction) {
        p1 = Utils::pointScale(oldp1, c, s);
        p2 = Utils::pointScale(oldp2, c, s);
    }
    else {
        p1 = Utils::pointScale(p1, c, s);
        p2 = Utils::pointScale(p2, c, s);
    }
}

QRect Line::getRectHull()
{
    return QRect(p1, p2).normalized();
}
