#ifndef LINE_H
#define LINE_H

#include "shape.h"

#include <QColor>

namespace cg {

class Line : public Shape
{
public:
    Line(const QPoint &point1, const QPoint &point2,
         const QColor &color, const QString &algorithm);
    ~Line() = default;

    QString shapeName() { return "Line"; }

    void beginTransaction();
    void commitTransaction();
    void rollbackTransaction();

    void draw(QImage &canvas);
    void translate(const QPoint &d);
    void scale(const QPoint &c, double s);
    void rotate(const QPoint &c, double r);
    cg::Shape *clip(const QPoint &p1, const QPoint &p2, const QString &alg);

    QRect getRectHull();

private:
    void drawByDefault(QImage &canvas);
    void drawByDDA(QImage &canvas);
    void drawByBresenham(QImage &canvas);
    void setPixel(QImage &canvas, int x, int y);

    cg::Shape *clipByDefault(const QPoint &topLeft, const QPoint &bottomRight);
    cg::Shape *clipByCohenSutherland(const QPoint &topLeft,
                                     const QPoint &bottomRight);
    static int calcOutCode(int x, int y, int top, int bottom, int left, int right);

    cg::Shape *clipByLiangBarsky(const QPoint &topLeft,
                                 const QPoint &bottomRight);
    static double max(double a[], int n, double defaultval = 0.0);
    static double min(double a[], int n, double defaultval = 1.0);

    QPoint p1, p2;
    QColor c;
    QString alg;

    /* for transaction */
    QPoint oldp1, oldp2;
};

}

#endif // LINE_H
