#ifndef POLYGON_H
#define POLYGON_H

#include "shape.h"

#include <QVector>
#include <QColor>

namespace CG {

class Polygon : public Shape
{
public:
    Polygon(const QVector<QPoint> &points,
            const QColor &color, const QString &algorithm);
    ~Polygon() = default;

    QString shapeName() { return "Polygon"; }

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
    void drawByDDA(QImage &canvas);
    void drawByBresenham(QImage &canvas);

    QVector<QPoint> vp;
    QColor c;
    QString alg;

    /* for transaction */
    QVector<QPoint> oldvp;
};

}

#endif // POLYGON_H
