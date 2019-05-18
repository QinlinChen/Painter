#include "polygon.h"
#include "utils.h"
#include "line.h"

#include <QPainter>
#include <QDebug>
namespace cg {

Polygon::Polygon(const QVector<QPoint> &points,
                 const QColor &color, const QString &algorithm)
    : vp(points), c(color), alg(algorithm)
{
    Q_ASSERT(vp.size() >= 3);
}

void Polygon::beginTransaction()
{
    oldvp = vp;
    Shape::beginTransaction();
}

void Polygon::commitTransaction()
{
    Shape::commitTransaction();
}

void Polygon::rollbackTransaction()
{
    vp = oldvp;
    Shape::rollbackTransaction();
}

void Polygon::draw(QImage &canvas)
{
    if (alg == "DDA")
        drawByDDA(canvas);
    else if (alg == "Bresenham")
        drawByBresenham(canvas);
    else
        drawByDefault(canvas);
}

void Polygon::drawByDefault(QImage &canvas)
{
    QPainter painter(&canvas);
    painter.setPen(c);
    painter.drawPolygon(vp.data(), vp.size());
}

void Polygon::drawByDDA(QImage &canvas)
{
    Q_ASSERT(vp.size() >= 3);

    for (int i = 0; i < vp.size() - 1; ++i)
        cg::Line(vp[i], vp[i+1], c, "DDA").draw(canvas);
    cg::Line(vp.back(), vp.front(), c, "DDA").draw(canvas);
}

void Polygon::drawByBresenham(QImage &canvas)
{
    Q_ASSERT(vp.size() >= 3);

    for (int i = 0; i < vp.size() - 1; ++i)
        cg::Line(vp[i], vp[i+1], c, "Bresenham").draw(canvas);
    cg::Line(vp.back(), vp.front(), c, "Bresenham").draw(canvas);
}

void Polygon::translate(const QPoint &d)
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

void Polygon::scale(const QPoint &c, double s)
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

void Polygon::rotate(const QPoint &c, double r)
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

QRect Polygon::getRectHull()
{
    Q_ASSERT(vp.size() >= 3);
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
