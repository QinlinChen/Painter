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
    drawByBspline(canvas);
}

void Curve::drawByBezier(QImage &canvas)
{
    Q_ASSERT(vp.size() >= 2);
    QPoint prev = calcCasteljauPoint(vp, 0.0);
    QPoint cur;

    double step = 0.001;
    for (double u = step; u <= 1; u += step) {
        cur = calcCasteljauPoint(vp, u);
        Line(prev, cur, c, "").draw(canvas);
        prev = cur;
    }
}

QPoint Curve::calcCasteljauPoint(const QVector<QPoint> &points, double u)
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

    int order = vp.size() - 1;
    QVector<double> knots = createKnots(vp.size(), order);

    for (double u = 0; u <= 1; u += 0.001) {
        QPoint deBoorPoint = calcDeBoorPoint(vp, knots, u, order);
        canvas.setPixel(deBoorPoint, c.rgb());
    }
}

QPoint Curve::calcDeBoorPoint(const QVector<QPoint> &controls,
                              const QVector<double> &knots,
                              double u, int order)
{
    QVector<QPointF> vp;
    for (auto &controlPoint : controls)
        vp.append(controlPoint);

    int n = controls.size() - 1;
    for (int r = 1; r <= order - 1; ++r) {
        for (int i = n; i >= r; --i) {
            double lamda = (u - knots[i]) / (knots[i + order - r] - knots[i]);
            vp[i] = lamda * vp[i] + (1 - lamda) * vp[i - 1];
        }
    }

    int knotIndex = findKnotIndex(knots, controls.size(), order, u);
    const QPointF &deBoorPoint = vp[knotIndex];
    return QPoint(qRound(deBoorPoint.x()), qRound(deBoorPoint.y()));
}

QVector<double> Curve::createKnots(int nControl, int order)
{
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

    qDebug() << knots;
    return knots;
}

int Curve::findKnotIndex(const QVector<double> &knots,
                         int nControl, int order, double u)
{
    double step = 1.0 / (nControl - order + 1);
    int knotIndex = order + static_cast<int>(u / step);

    Q_ASSERT(u >= knots[knotIndex]);
    Q_ASSERT(u <= knots[knotIndex + 1]);
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
