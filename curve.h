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
    static QPoint calcDeCasteljauPoint(double u, const QVector<QPoint> &points);
    static double calcLength(const QVector<QPoint> &points);

    void drawByBspline(QImage &canvas);
    static QPoint calcDeBoorPoint(double u, int order,
                                  const QVector<QPoint> &controls,
                                  const QVector<double> &knots);
    static QVector<double> createKnots(int nControl, int k);
    static int calcKnotIndex(double u, int nControl, int order);

    QVector<QPoint> vp;
    QColor c;
    QString alg;

    QVector<QPoint> oldvp;
};

}

#endif // CURVE_H
