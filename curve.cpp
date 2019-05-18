#include "curve.h"
#include "line.h"
#include "utils.h"

#include <QDebug>

namespace cg {

Curve::Curve(const QVector<QPoint> &points,
             const QColor &color, const QString &algorithm)
    : vp(points), c(color), alg(algorithm)
{
    Q_ASSERT(vp.size() >= 2);
}

void Curve::beginTransaction()
{
    oldvp = vp;
    Shape::beginTransaction();
}

void Curve::commitTransaction()
{
    Shape::commitTransaction();
}

void Curve::rollbackTransaction()
{
    vp = oldvp;
    Shape::rollbackTransaction();
}

void Curve::draw(QImage &canvas)
{
    if (alg == "Bezier")
        drawByBezier(canvas);
    else if (alg == "B-spline")
        drawByBspline(canvas);
    else
        drawByDefault(canvas);
}

void Curve::drawByDefault(QImage &canvas)
{
    Q_ASSERT(vp.size() >= 2);
    for (int i = 0; i < vp.size() - 1; ++i)
        cg::Line(vp[i], vp[i+1], c, "DDA").draw(canvas);
}

void Curve::drawByBezier(QImage &canvas)
{
    // TODO
}

void Curve::drawByBspline(QImage &canvas)
{
    // TODO
}

void Curve::translate(const QPoint &d)
{
    if (duringTransaction) {
        Q_ASSERT(vp.size() == oldvp.size());
        for (int i = 0; i < vp.size(); ++i)
            vp[i] = oldvp[i] + d;
    }
    else {
        for (auto &p : vp)
            p += d;
    }
}

void Curve::scale(const QPoint &c, double s)
{
    if (duringTransaction) {
        Q_ASSERT(vp.size() == oldvp.size());
        for (int i = 0; i < vp.size(); ++i)
            vp[i] = utils::scalePoint(oldvp[i], c, s);
    }
    else {
        for (int i = 0; i < vp.size(); ++i)
            vp[i] = utils::scalePoint(vp[i], c, s);
    }
}

void Curve::rotate(const QPoint &c, double r)
{
    if (duringTransaction) {
        Q_ASSERT(vp.size() == oldvp.size());
        for (int i = 0; i < vp.size(); ++i)
            vp[i] = utils::rotatePoint(oldvp[i], c, r);
    }
    else {
        for (int i = 0; i < vp.size(); ++i)
            vp[i] = utils::rotatePoint(vp[i], c, r);
    }
}

QRect Curve::getRectHull()
{
    Q_ASSERT(vp.size() >= 2);
    QPoint topLeft = vp[0];
    QPoint bottomRight = vp[0];
    for (int i = 1; i < vp.size(); ++i) {
        const QPoint &p = vp[i];
        topLeft.rx() = qMin(topLeft.x(), p.x());
        topLeft.ry() = qMin(topLeft.y(), p.y());
        bottomRight.rx() = qMax(bottomRight.x(), p.x());
        bottomRight.ry() = qMax(bottomRight.y(), p.y());
    }
    return QRect(topLeft, bottomRight);
}

}
