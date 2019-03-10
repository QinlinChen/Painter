#ifndef UTILS_H
#define UTILS_H

#include <QPoint>
#include <QRect>

class Utils
{
public:
    static QRect getRectAroundPoint(const QPoint &point, int radius);
    static bool isClose(const QPoint &p1, const QPoint &p2, int radius);
};

#endif // UTILS_H
