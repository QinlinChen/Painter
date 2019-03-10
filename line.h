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
    void draw(QImage &canvas);
    void translate(int dx, int dy);
    void rotate(int x, int y, float r);
    void scale(int x, int y, float s);
    QRect getRectHull();
    QPoint getCenter();
    void setCenter(int x, int y);

private:
    QPoint p1, p2;
    QColor c;
    QString alg;

    QPoint center;
    bool autoCenter;
};

#endif // LINE_H
