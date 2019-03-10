#ifndef SHAPE_H
#define SHAPE_H

#include <QString>
#include <QRect>
#include <QPoint>

class QImage;

class Shape
{
public:
    Shape();
    virtual ~Shape() = default;

    virtual QString shapeName() = 0;

    virtual void beginTransaction();
    virtual void commitTransaction();
    virtual void rollbackTransaction();

    virtual void draw(QImage &canvas) = 0;
    virtual void translate(const QPoint &d) = 0;
    virtual void rotate(const QPoint &c, double r) = 0;
    virtual void scale(const QPoint &c, double s) = 0;

    virtual QRect getRectHull() = 0;

    virtual QPoint getCenter();
    virtual void setCenter(const QPoint &newCenter);

protected:
    bool duringTransaction;

    QPoint center;
    bool autoCenter;
};

#endif // SHAPE_H
