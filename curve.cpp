#include "curve.h"
#include "line.h"
#include "utils.h"

#include <QImage>
#include <QtMath>
#include <QtDebug>

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
    drawByBezier(canvas);
}

void Curve::drawByBezier(QImage &canvas)
{
    Q_ASSERT(vp.size() >= 2);

    QPoint prev = calcDeCasteljauPoint(0.0, vp);
    QPoint cur;

    double step = 0.001;
    for (double u = step; u <= 1; u += step) {
        cur = calcDeCasteljauPoint(u, vp);
        Line(prev, cur, c, "").draw(canvas);
        prev = cur;
    }
}

QPoint Curve::calcDeCasteljauPoint(double u, const QVector<QPoint> &points)
{
    QVector<QPointF> vp;
    for (auto &point : points)
        vp.append(QPointF(point));

    int n = vp.size() - 1;
    for (int r = 0; r < n; ++r)
        for (int i = 0; i < n - r; ++i)
            vp[i] = (1 - u) * vp[i] + u * vp[i + 1];

    const QPointF &casteljauPoint = vp.front();
    return QPoint(qRound(casteljauPoint.x()), qRound(casteljauPoint.y()));
}

double Curve::calcLength(const QVector<QPoint> &points)
{
    double length = 0.0;

    for (int i = 0; i < points.size() - 1; ++i) {
        QPoint dist = points[i + 1] - points[i];
        length += qSqrt(QPoint::dotProduct(dist, dist));
    }

    return length;
}

void Curve::drawByBspline(QImage &canvas)
{
    Q_ASSERT(vp.size() >= 2);
    if (vp.size() == 2) {
        cg::Line(vp[0], vp[1], c, "").draw(canvas);
        return;
    }

    int nControl = vp.size();
    int order = nControl <= 4 ? nControl - 1 : 4;
    QVector<double> knots = createKnots(nControl, order);

    QPoint prev = calcDeBoorPoint(0.0, order, vp, knots);
    QPoint cur;

    double step = 0.001;
    for (double u = step; u <= 1; u += step) {
        cur = calcDeBoorPoint(u, order, vp, knots);
        Line(prev, cur, c, "").draw(canvas);
        prev = cur;
    }
}

QPoint Curve::calcDeBoorPoint(double u, int order,
                              const QVector<QPoint> &controls,
                              const QVector<double> &knots)
{
    QVector<QPointF> cp;
    for (auto &controlPoint : controls)
        cp.append(controlPoint);

    int n = controls.size() - 1;
    for (int r = 1; r <= order - 1; ++r) {
        for (int i = n; i >= r; --i) {
            double lamda = (u - knots[i]) / (knots[i + order - r] - knots[i]);
            cp[i] = lamda * cp[i] + (1 - lamda) * cp[i - 1];
        }
    }

    int knotIndex = calcKnotIndex(u, controls.size(), order);
    Q_ASSERT(u >= knots[knotIndex]);
    Q_ASSERT(u <= knots[knotIndex + 1]);
    const QPointF &deBoorPoint = cp[knotIndex];
    return QPoint(qRound(deBoorPoint.x()), qRound(deBoorPoint.y()));
}

QVector<double> Curve::createKnots(int nControl, int order)
{
    Q_ASSERT(order <= nControl);

    int nKnot = nControl + order;
    QVector<double> knots(nKnot);

    for (int i = 0; i < nKnot; ++i) {
        if (i < order)
            knots[i] = 0.0;
        else if (i > nControl)
            knots[i] = 1.0;
        else
            knots[i] = knots[i - 1] + 1.0 / (nControl - order + 1);
    }

    return knots;
}

int Curve::calcKnotIndex(double u, int nControl, int order)
{
    Q_ASSERT(order <= nControl);

    double step = 1.0 / (nControl - order + 1);
    int knotIndex = order - 1 + static_cast<int>(u / step);
    return knotIndex;
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
