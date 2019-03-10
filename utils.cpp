#include "utils.h"

#include <QtMath>

QRect Utils::getRectAroundPoint(const QPoint &point, int radius)
{
    return QRect(point.x() - radius, point.y() - radius,
                 2 * radius, 2 * radius).normalized();
}

bool Utils::isClose(const QPoint &p1, const QPoint &p2, int radius)
{
    QPoint dist = p1 - p2;
    return qAbs(dist.x()) <= radius && qAbs(dist.y()) <= radius;
}

QPoint Utils::scalePoint(const QPoint &p, const QPoint &center, double s)
{
    int x = static_cast<int>(s * (p.x() - center.x()) + center.x());
    int y = static_cast<int>(s * (p.y() - center.y()) + center.y());
    return QPoint(x, y);
}

QPoint Utils::rotatePoint(const QPoint &p, const QPoint &center, double r)
{
    double cosTheta = qCos(r);
    double sinTheta = qSin(r);
    QPoint v = p - center;
    int x = static_cast<int>(center.x() + cosTheta * v.x() - sinTheta * v.y());
    int y = static_cast<int>(center.y() + sinTheta * v.x() + cosTheta * v.y());
    return QPoint(x, y);
}
