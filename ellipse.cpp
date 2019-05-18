#include "ellipse.h"
#include "utils.h"
#include "line.h"

#include <QImage>
#include <QPainter>
#include <QtMath>
#include <QtDebug>

namespace cg {

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
    rx = qAbs(p.x() - topleft.x());
    ry = qAbs(p.y() - topleft.y());
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
    drawByBresenham(canvas);
}

void Ellipse::drawByDefault(QImage &canvas)
{
    QPainter painter(&canvas);
    painter.setPen(c);
    painter.drawEllipse(p, rx, ry);
}

void Ellipse::drawByBresenham(QImage &canvas)
{
    if (rx == 0 && ry == 0) {
        canvas.setPixel(p, c.rgb());
        return;
    }
    if (rx == 0) {
        QPoint p1 = p + QPoint(0, ry);
        QPoint p2 = p - QPoint(0, ry);
        cg::Line(p1, p2, c, "").draw(canvas);
        return;
    }
    if (ry == 0) {
        QPoint p1 = p + QPoint(rx, 0);
        QPoint p2 = p - QPoint(rx, 0);
        cg::Line(p1, p2, c, "").draw(canvas);
        return;
    }

    int x = 0, y = ry;
    int rxSquared = rx * rx, rySquared = ry * ry;

    int decisionParam = 4 * (rySquared - rxSquared * ry) + rxSquared;
    int deltaDecisionParamIfLe = 4 * (rySquared * (2 * x + 1));
    int deltaDecisionParamIfG = 4 * (rySquared * (2 * x + 1) - rxSquared * 2 * y);
    while (rySquared * x < rxSquared * y) {
        setSymmetricPixel(canvas, x, y);
        if (decisionParam <= 0) {
            ++x;
            deltaDecisionParamIfLe += 8 * rySquared;
            deltaDecisionParamIfG += 8 * rySquared;
            decisionParam += deltaDecisionParamIfLe;
        }
        else {
            ++x;
            --y;
            deltaDecisionParamIfLe += 8 * rySquared;
            deltaDecisionParamIfG += 8 * (rySquared + rxSquared);
            decisionParam += deltaDecisionParamIfG;
        }
    }

    decisionParam = rySquared * (2 * x + 1) * (2 * x + 1)
            + 4 * (rxSquared * (y - 1) * (y - 1) - rxSquared * rySquared);
    deltaDecisionParamIfLe = 4 * (rySquared * 2 * x - rxSquared * (2 * y - 1));
    deltaDecisionParamIfG = 4 * (-rxSquared * (2 * y - 1));
    while (y >= 0) {
        setSymmetricPixel(canvas, x, y);
        if (decisionParam <= 0) {
            ++x;
            --y;
            deltaDecisionParamIfLe += 8 * (rySquared + rxSquared);
            deltaDecisionParamIfG += 8 * rxSquared;
            decisionParam += deltaDecisionParamIfLe;
        }
        else {
            --y;
            deltaDecisionParamIfLe += 8 * rxSquared;
            deltaDecisionParamIfG += 8 * rxSquared;
            decisionParam += deltaDecisionParamIfG;
        }
    }
}

void Ellipse::setSymmetricPixel(QImage &canvas, int x, int y)
{
    Q_ASSERT(x >= 0);
    Q_ASSERT(y >= 0);
    canvas.setPixel(p.x() + x, p.y() + y, c.rgb());
    canvas.setPixel(p.x() + x, p.y() - y, c.rgb());
    canvas.setPixel(p.x() - x, p.y() + y, c.rgb());
    canvas.setPixel(p.x() - x, p.y() - y, c.rgb());
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
        p = utils::scalePoint(oldp, c, s);
        rx = static_cast<int>(oldrx * s);
        ry = static_cast<int>(oldry * s);
    }
    else {
        p = utils::scalePoint(p, c, s);
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
                p = utils::rotatePoint(oldp, c, r);
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
                p = utils::rotatePoint(p, c, r);
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
