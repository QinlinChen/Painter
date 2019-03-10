#ifndef UTILS_H
#define UTILS_H

#include <QPoint>
#include <QRect>

class Utils
{
public:
    static QRect getRectAroundPoint(const QPoint &point, int radius);
    static bool isClose(const QPoint &p1, const QPoint &p2, int radius);

    static QPoint pointScale(const QPoint &p, const QPoint &center, float s);
};

#endif // UTILS_H