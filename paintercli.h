#ifndef PAINTERCLI_H
#define PAINTERCLI_H

#include <QString>
#include <QImage>
#include <QColor>
#include <QPoint>
#include <QVector>

class PainterCLI
{
public:
    PainterCLI() = default;

    int exec(int argc, char *argv[]);

private:
    void resetCanvas(int width, int height);
    void saveCanvas(const QString &name);
    void setColor(const QColor &color);
    void drawLine(int id, const QPoint &p1, const QPoint &p2,
                  const QString &alg);
    void drawPolygon(int id, const QVector<QPoint> &points,
                     const QString &alg);
    void drawEllipse(int id, const QPoint &center, int rx, int ry);
    void drawCurve(int id, const QVector<QPoint> &points,
                   const QString &alg);
    void translate(int id, const QPoint &d);
    void rotate(int id, const QPoint &c, double r);
    void scale(int id, const QPoint &c, double s);
    void clip(int id, const QPoint &topLeft, const QPoint &bottomRight,
              const QString &alg);

    QColor color;
    QImage canvas;
};

#endif // PAINTERCLI_H
