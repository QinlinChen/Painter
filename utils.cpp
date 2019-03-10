#include "utils.h"

QRect Utils::getRectAroundPoint(const QPoint &point, int radius)
{
    return QRect(point.x() - radius, point.y() - radius,
                 2 * radius, 2 * radius).normalized();
}

bool Utils::isClose(const QPoint &p1, const QPoint &p2, int radius)
{
    QPoint dist = p1 - p2;
    return qAbs(dist.x()) <= radius || qAbs(dist.y()) <= radius;
}
