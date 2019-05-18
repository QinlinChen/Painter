#ifndef UTILS_H
#define UTILS_H

#include <QPoint>
#include <QRect>

namespace utils {

QRect getRectAroundPoint(const QPoint &point, int radius);
bool isClose(const QPoint &p1, const QPoint &p2, int radius);
int innerProd(const QPoint &p1, const QPoint &p2);
int crossProd(const QPoint &p1, const QPoint &p2);
QPoint scalePoint(const QPoint &p, const QPoint &center, double s);
QPoint rotatePoint(const QPoint &p, const QPoint &center, double r);

}

#endif // UTILS_H
