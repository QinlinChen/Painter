#ifndef CURVE_H
#define CURVE_H

#include "shape.h"

#include <QVector>
#include <QColor>

namespace cg {

class Curve : public Shape
{
public:
    Curve(const QVector<QPoint> &points,
          const QColor &color, const QString &algorithm);
    ~Curve() = default;

    QString shapeName() { return "Curve"; }

    void beginTransaction();
    void commitTransaction();
    void rollbackTransaction();

    void draw(QImage &canvas);
    void translate(const QPoint &d);
    void scale(const QPoint &c, double s);
    void rotate(const QPoint &c, double r);

    QRect getRectHull();

private:
    void drawByDefault(QImage &canvas);
    void drawByBezier(QImage &canvas);
    void drawByBspline(QImage &canvas);

    QVector<QPoint> vp;
    QColor c;
    QString alg;

    QVector<QPoint> oldvp;
};

}

#endif // CURVE_H
