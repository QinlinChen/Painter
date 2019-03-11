#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "shape.h"

#include <QColor>

namespace CG {

class Ellipse : public Shape
{
public:
    Ellipse(const QPoint &center, int rx, int ry,
            const QColor &color, const QString &algorithm);
    Ellipse(const QPoint &topleft, const QPoint &bottomRight,
            const QColor &color, const QString &algorithm);
    ~Ellipse() = default;

    QString shapeName() { return "Ellipse"; }

    void beginTransaction();
    void commitTransaction();
    void rollbackTransaction();

    void draw(QImage &canvas);
    void translate(const QPoint &d);
    void scale(const QPoint &c, double s);
    void rotate(const QPoint &c, double r);

    QRect getRectHull();

private:
    QPoint p;
    int rx, ry;
    QColor c;
    QString alg;

    /* for transaction */
    QPoint oldp;
    int oldrx, oldry;
};

}

#endif // ELLIPSE_H
