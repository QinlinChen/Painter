#ifndef UTILS_H
#define UTILS_H

#include <QPoint>
#include <QRect>

class Utils
{
public:
    static QRect getRectAroundPoint(const QPoint &point, int radius);
    static bool isClose(const QPoint &p1, const QPoint &p2, int radius);

    static int innerProd(const QPoint &p1, const QPoint &p2) {
        return p1.x() * p2.x() + p1.y() * p2.y();
    }

    static int crossProd(const QPoint &p1, const QPoint &p2)  {
        return p1.x() * p2.y() - p2.x() * p1.y();
    }

    static QPoint scalePoint(const QPoint &p, const QPoint &center, double s);
    static QPoint rotatePoint(const QPoint &p, const QPoint &center, double r);
};

#endif // UTILS_H
