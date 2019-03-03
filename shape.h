#ifndef SHAPE_H
#define SHAPE_H

class QImage;

class Shape
{
public:
    Shape() = default;
    virtual ~Shape() = default;

    virtual void draw(QImage &canvas) = 0;
    virtual void translate(int dx, int dy) = 0;
    virtual void rotate(int x, int y, int r) = 0;
    virtual void scale(int x, int y, int s) = 0;
};

#endif // SHAPE_H
