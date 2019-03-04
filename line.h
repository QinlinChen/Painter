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

    void draw(QImage &canvas);
    void translate(int dx, int dy);
    void rotate(int x, int y, int r);
    void scale(int x, int y, int s);
    QString shapeName();

private:
    QPoint p1, p2;
    QColor c;
    QString alg;
};

#endif // LINE_H
