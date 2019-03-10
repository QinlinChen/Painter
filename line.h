#ifndef LINE_H
#define LINE_H

#include "shape.h"

#include <QPoint>
#include <QColor>
#include <QString>

class Line : public Shape
{
public:
    Line(const QPoint &point1, const QPoint &point2,
         const QColor &color, const QString &algorithm);
    ~Line() = default;

    QString shapeName();

    void beginTransaction();
    void commitTransaction();
    void rollbackTransaction();

    void draw(QImage &canvas);
    void translate(const QPoint &d);
    void rotate(const QPoint &c, float r);
    void scale(const QPoint &c, float s);

    QRect getRectHull();

private:
    QPoint p1, p2;
    QColor c;
    QString alg;

    /* for transaction */
    QPoint oldp1, oldp2;
};

#endif // LINE_H
