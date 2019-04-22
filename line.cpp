#include "line.h"
#include "utils.h"

#include <QPainter>
#include <QtDebug>

namespace CG {

Line::Line(const QPoint &point1, const QPoint &point2,
           const QColor &color, const QString &algorithm)
    : p1(point1), p2(point2), c(color), alg(algorithm)
{

}

void Line::beginTransaction()
{
    oldp1 = p1;
    oldp2 = p2;
    Shape::beginTransaction();
}

void Line::commitTransaction()
{
    Shape::commitTransaction();
}

void Line::rollbackTransaction()
{
    p1 = oldp1;
    p2 = oldp2;
    Shape::rollbackTransaction();
}

void Line::draw(QImage &canvas)
{
    if (alg == "DDA")
        drawByDDA(canvas);
    else if (alg == "Bresenham")
        drawByBresenham(canvas);
    else
        drawByDefault(canvas);
}

void Line::drawByDefault(QImage &canvas)
{
    QPainter painter(&canvas);
    painter.setPen(c);
    painter.drawLine(p1, p2);
}

void Line::drawByDDA(QImage &canvas)
{
    int x1 = p1.x(), x2 = p2.x();
    int y1 = p1.y(), y2 = p2.y();
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;

    if (qAbs(deltaY) <= qAbs(deltaX)) {
        if (deltaX == 0)
            return;

        int x = qMin(x1, x2);
        int xEnd = qMax(x1, x2);
        double y = static_cast<double>(x1 < x2 ? y1 : y2);
        double dy = static_cast<double>(deltaY) / deltaX;

        while (x != xEnd) {
            setPixel(canvas, x, qRound(y));
            y += dy;
            x++;
        }
    }
    else {
        Q_ASSERT(deltaY != 0);

        int y = qMin(y1, y2);
        int yEnd = qMax(y1, y2);
        double x = static_cast<double>(y1 < y2 ? x1 : x2);
        double dx = static_cast<double>(deltaX) / deltaY;

        while (y != yEnd) {
            setPixel(canvas, qRound(x), y);
            x += dx;
            y++;
        }
    }
}

void Line::drawByBresenham(QImage &canvas)
{
    /* Let y = mx + b be the line determined by p1 and p2. */
    int x1 = p1.x(), x2 = p2.x();
    int y1 = p1.y(), y2 = p2.y();
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;


    /* |m| <= 1 */
    if (qAbs(deltaY) <= qAbs(deltaX)) {
        if (deltaX == 0)
            return;

        if (x1 > x2) {
            qSwap(x1, x2);
            qSwap(y1, y2);
        }
        int x = x1, y = y1;
        int dx = x2 - x1;
        int dy = y2 - y1;

        /* 0 <= m <= 1*/
        if (dy >= 0) {
            int decisionParam = 2 * dy - dx;
            for (; x != x2; ++x) {
                setPixel(canvas, x, y);
                if (decisionParam >= 0) {
                    ++y;
                    decisionParam += 2 * (dy - dx);
                }
                else {
                    decisionParam += 2 * dy;
                }
            }
        }
        /* -1 <= m < 0 */
        else {
            int decisionParam = 2 * dy + dx;
            for (; x != x2; ++x) {
                setPixel(canvas, x, y);
                if (decisionParam >= 0) {
                    decisionParam += 2 * dy;
                }
                else {
                    --y;
                    decisionParam += 2 * (dy + dx);
                }
            }
        }
    }
    /* |m| > 1 */
    else {
        Q_ASSERT(deltaY != 0);

        if (y1 > y2) {
            qSwap(x1, x2);
            qSwap(y1, y2);
        }
        int x = x1, y = y1;
        int dx = x2 - x1;
        int dy = y2 - y1;

        /* m > 1 */
        if (dx >= 0) {
            int decisionParam = - (2 * dx - dy);
            for (; y != y2; ++y) {
                setPixel(canvas, x, y);
                if (decisionParam <= 0) {
                    ++x;
                    decisionParam -= 2 * (dx - dy);
                }
                else {
                    decisionParam -= 2 * dx;
                }
            }
        }
        /* m < -1 */
        else {
            int decisionParam = -(2 * dx + dy);
            for (; y != y2; ++y) {
                setPixel(canvas, x, y);
                if (decisionParam <= 0) {
                    decisionParam -= 2 * dx;
                }
                else {
                    --x;
                    decisionParam -= 2 * (dx + dy);
                }
            }
        }
    }
}

void Line::setPixel(QImage &canvas, int x, int y)
{
    canvas.setPixel(x, y, c.rgb());
}

void Line::translate(const QPoint &d)
{
    if (duringTransaction) {
        p1 = oldp1 + d;
        p2 = oldp2 + d;
    }
    else {
        p1 += d;
        p2 += d;
    }
}

void Line::scale(const QPoint &c, double s)
{
    if (duringTransaction) {
        p1 = Utils::scalePoint(oldp1, c, s);
        p2 = Utils::scalePoint(oldp2, c, s);
    }
    else {
        p1 = Utils::scalePoint(p1, c, s);
        p2 = Utils::scalePoint(p2, c, s);
    }
}

void Line::rotate(const QPoint &c, double r)
{
    if (duringTransaction) {
        p1 = Utils::rotatePoint(oldp1, c, r);
        p2 = Utils::rotatePoint(oldp2, c, r);
    }
    else {
        p1 = Utils::rotatePoint(p1, c, r);
        p2 = Utils::rotatePoint(p2, c, r);
    }
}

void Line::clip(const QPoint &topLeft, const QPoint &bottomRight,
                const QString &alg)
{
    // TODO
    qDebug() << "clip(" << topLeft << "," << bottomRight << "," << alg << ")";
}

QRect Line::getRectHull()
{
    return QRect(p1, p2).normalized();
}

}
