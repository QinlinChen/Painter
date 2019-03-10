#ifndef SHAPE_H
#define SHAPE_H

#include <QString>
#include <QRect>
#include <QPoint>

class QImage;

class Shape
{
public:
    Shape() = default;
    virtual ~Shape() = default;

    virtual QString shapeName() = 0;
    virtual void draw(QImage &canvas) = 0;
    virtual void translate(int dx, int dy) = 0;
    virtual void rotate(int x, int y, float r) = 0;
    virtual void scale(int x, int y, float s) = 0;
    virtual QRect getRectHull() = 0;
    virtual QPoint getCenter() = 0;
    virtual void setCenter(int x, int y) = 0;
};

#endif // SHAPE_H
