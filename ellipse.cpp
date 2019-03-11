#include "ellipse.h"
#include "utils.h"

#include <QPainter>
#include <QtMath>

namespace CG {

Ellipse::Ellipse(const QPoint &center, int rx, int ry,
                 const QColor &color, const QString &algorithm)
    : p(center), c(color), alg(algorithm)
{
    this->rx = rx;
    this->ry = ry;
}

Ellipse::Ellipse(const QPoint &topleft, const QPoint &bottomRight,
                 const QColor &color, const QString &algorithm)
    : c(color), alg(algorithm)
{
    p = (topleft + bottomRight) / 2;
    rx = (bottomRight.x() - topleft.x()) / 2;
    ry = (bottomRight.y() - topleft.y()) / 2;
}

void Ellipse::beginTransaction()
{
    oldp = p;
    oldrx = rx;
    oldry = ry;
    Shape::beginTransaction();
}

void Ellipse::commitTransaction()
{
    Shape::commitTransaction();
}

void Ellipse::rollbackTransaction()
{
    p = oldp;
    rx = oldrx;
    ry = oldry;
    Shape::rollbackTransaction();
}

void Ellipse::draw(QImage &canvas)
{
    QPainter painter(&canvas);
    painter.setPen(c);
    painter.drawEllipse(p, rx, ry);
}

void Ellipse::translate(const QPoint &d)
{
    if (duringTransaction) {
        p = oldp + d;
    }
    else {
        p += d;
    }
}

void Ellipse::scale(const QPoint &c, double s)
{
    if (duringTransaction) {
        p = Utils::scalePoint(oldp, c, s);
        rx = static_cast<int>(oldrx * s);
        ry = static_cast<int>(oldry * s);
    }
    else {
        p = Utils::scalePoint(p, c, s);
        rx *= s;
        ry *= s;
    }
}

void Ellipse::rotate(const QPoint &c, double r)
{
    static const double aTheta[] = { -M_PI_2, 0.0, M_PI_2, M_PI };
    static const int aThetaSize = sizeof(aTheta) / sizeof(aTheta[0]);

    for (int i = 0; i < aThetaSize; ++i) {
        if (qAbs(aTheta[i] - r) <= 0.1) {
            if (duringTransaction) {
                p = Utils::rotatePoint(oldp, c, r);
                if (i % 2 == 0) {
                    rx = oldry;
                    ry = oldrx;
                }
                else {
                    rx = oldrx;
                    ry = oldry;
                }
            }
            else {
                p = Utils::rotatePoint(p, c, r);
                if (i % 2 == 0) {
                    std::swap(rx, ry);
                }
            }
        }
    }
}

QRect Ellipse::getRectHull()
{
    return QRect(p.x() - rx, p.y() - ry, 2 * rx, 2 * ry);
}

}
