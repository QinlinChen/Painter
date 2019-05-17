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
            for (; x <= x2; ++x) {
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
            for (; x <= x2; ++x) {
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
            for (; y <= y2; ++y) {
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
            for (; y <= y2; ++y) {
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

CG::Shape *Line::clip(const QPoint &p1, const QPoint &p2, const QString &alg)
{
    QRect rect(p1, p2);
    rect = rect.normalized();

    if (alg == "Cohen-Sutherland")
        return clipByCohenSutherland(rect.topLeft(), rect.bottomRight());
    if (alg == "Liang-Barsky")
        return clipByLiangBarsky(rect.topLeft(), rect.bottomRight());
    return clipByDefault(rect.topLeft(), rect.bottomRight());
}

CG::Shape *Line::clipByDefault(const QPoint &topLeft, const QPoint &bottomRight)
{
    return clipByLiangBarsky(topLeft, bottomRight);
}

#define TOP_MASK      0x8
#define BOTTOM_MASK   0x4
#define LEFT_MASK     0x2
#define RIGHT_MASK    0x1
#define IS_INSIDE_WINDOW(CODE1, CODE2)  (!((CODE1) | CODE2))
#define IS_OUTSIDE_WINDOW(CODE1, CODE2) ((CODE1) & (CODE2))

CG::Shape *Line::clipByCohenSutherland(const QPoint &topLeft,
                                       const QPoint &bottomRight)
{
    int top = topLeft.y(), bottom = bottomRight.y();
    int left = topLeft.x(), right = bottomRight.x();
    int x1 = p1.x(), y1 = p1.y();
    int x2 = p2.x(), y2 = p2.y();
    int outCode1 = calcOutCode(x1, y1, top, bottom, left, right);
    int outCode2 = calcOutCode(x2, y2, top, bottom, left, right);
    bool clipAll = false;

    while (true) {
        if (IS_INSIDE_WINDOW(outCode1, outCode2)) {
            break;
        }
        else if (IS_OUTSIDE_WINDOW(outCode1, outCode2)) {
            clipAll = true;
            break;
        }
        else {
            double xNew = 0.0, yNew = 0.0;
            int outCodeSelected = outCode1 ? outCode1 : outCode2;

            if (outCodeSelected & LEFT_MASK) {
                yNew = y1 + (y2 - y1) * (left - x1) / (x2 - x1);
                xNew = left;
            } else if (outCodeSelected & RIGHT_MASK) {
                yNew = y1 + (y2 - y1) * (right - x1) / (x2 - x1);
                xNew = right;
            } else if (outCodeSelected & TOP_MASK) {
                xNew = x1 + (x2 - x1) * (top - y1) / (y2 - y1);
                yNew = top;
            } else if (outCodeSelected & BOTTOM_MASK) {
                xNew = x1 + (x2 - x1) * (bottom - y1) / (y2 - y1);
                yNew = bottom;
            } else {
                Q_ASSERT(0); /* Should not reach here */
            }

            if (outCodeSelected == outCode1) {
                x1 = qRound(xNew);
                y1 = qRound(yNew);
                outCode1 = calcOutCode(x1, y1, top, bottom, left, right);
            } else {
                x2 = qRound(xNew);
                y2 = qRound(yNew);
                outCode2 = calcOutCode(x2, y2, top, bottom, left, right);
            }
        }
    }

    if (clipAll)
        return nullptr;
    return new Line(QPoint(x1, y1), QPoint(x2, y2), c, alg);
}

int Line::calcOutCode(int x, int y, int top, int bottom, int left, int right)
{
    Q_ASSERT(top < bottom);
    Q_ASSERT(left < right);

    int outCode = 0;

    if (y < top)
        outCode |= TOP_MASK;
    else if (y > bottom)
        outCode |= BOTTOM_MASK;
    if (x < left)
        outCode |= LEFT_MASK;
    else if (x > right)
        outCode |= RIGHT_MASK;

    return outCode;
}

CG::Shape *Line::clipByLiangBarsky(const QPoint &topLeft,
                                   const QPoint &bottomRight)
{
    int top = topLeft.y(), bottom = bottomRight.y();
    int left = topLeft.x(), right = bottomRight.x();
    Q_ASSERT(top < bottom);
    Q_ASSERT(left < right);

    int x1 = p1.x(), y1 = p1.y();
    int x2 = p2.x(), y2 = p2.y();

    int p1 = -(x2 - x1);
    int p2 = -p1;
    int p3 = -(y2 - y1);
    int p4 = -p3;

    int q1 = x1 - left;
    int q2 = right - x1;
    int q3 = y1 - top;
    int q4 = bottom - y1;

    if ((p1 == 0 && q1 < 0) || (p3 == 0 && q3 < 0)) {
        /* The line is parallel and outside to the clipping window. */
        return nullptr;
    }

    double posarr[4], negarr[4];
    int posind = 0, negind = 0;

    if (p1 != 0) {
        double r1 = static_cast<double>(q1) / p1;
        double r2 = static_cast<double>(q2) / p2;
        if (p1 < 0) {
            negarr[negind++] = r1;
            posarr[posind++] = r2;
        } else {
            negarr[negind++] = r2;
            posarr[posind++] = r1;
        }
    }
    if (p3 != 0) {
        double r3 = static_cast<double>(q3) / p3;
        double r4 = static_cast<double>(q4) / p4;
        if (p3 < 0) {
            negarr[negind++] = r3;
            posarr[posind++] = r4;
        } else {
            negarr[negind++] = r4;
            posarr[posind++] = r3;
        }
    }

    double u1 = max(negarr, negind, 0.0);
    double u2 = min(posarr, posind, 1.0);

    if (u1 > u2) {
        return nullptr;
    }

    QPoint p1New(static_cast<int>(x1 + p2 * u1) , static_cast<int>(y1 + p4 * u1));
    QPoint p2New(static_cast<int>(x1 + p2 * u2), static_cast<int>(y1 + p4 * u2));
    return new Line(p1New, p2New, c, alg);
}

double Line::max(double a[], int n, double defaultval)
{
    double m = defaultval;
    for (int i = 0; i < n; ++i)
        if (a[i] > m)
            m = a[i];
    return m;
}

double Line::min(double a[], int n, double defaultval)
{
    double m = defaultval;
    for (int i = 0; i < n; ++i)
        if (a[i] < m)
            m = a[i];
    return m;
}

QRect Line::getRectHull()
{
    return QRect(p1, p2).normalized();
}

}
