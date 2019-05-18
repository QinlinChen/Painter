#include "utils.h"

#include <QtMath>

namespace utils {

QRect getRectAroundPoint(const QPoint &point, int radius)
{
    return QRect(point.x() - radius, point.y() - radius,
                 2 * radius, 2 * radius).normalized();
}

bool isClose(const QPoint &p1, const QPoint &p2, int radius)
{
    QPoint dist = p1 - p2;
    return qAbs(dist.x()) <= radius && qAbs(dist.y()) <= radius;
}

int innerProd(const QPoint &p1, const QPoint &p2) {
    return p1.x() * p2.x() + p1.y() * p2.y();
}

int crossProd(const QPoint &p1, const QPoint &p2)  {
    return p1.x() * p2.y() - p2.x() * p1.y();
}

QPoint scalePoint(const QPoint &p, const QPoint &center, double s)
{
    int x = static_cast<int>(s * (p.x() - center.x()) + center.x());
    int y = static_cast<int>(s * (p.y() - center.y()) + center.y());
    return QPoint(x, y);
}

QPoint rotatePoint(const QPoint &p, const QPoint &center, double r)
{
    double cosTheta = qCos(r);
    double sinTheta = qSin(r);
    QPoint v = p - center;
    int x = static_cast<int>(center.x() + cosTheta * v.x() - sinTheta * v.y());
    int y = static_cast<int>(center.y() + sinTheta * v.x() + cosTheta * v.y());
    return QPoint(x, y);
}

}
