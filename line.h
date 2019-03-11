#ifndef LINE_H
#define LINE_H

#include "shape.h"

#include <QColor>

namespace CG {

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

    QRect getRectHull();

private:
    QPoint p1, p2;
    QColor c;
    QString alg;

    /* for transaction */
    QPoint oldp1, oldp2;
};

}

#endif // LINE_H
