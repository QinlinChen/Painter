#ifndef PAINTER_H
#define PAINTER_H

#include "shape.h"

#include <QWidget>
#include <QImage>
#include <QSize>
#include <QPoint>
#include <QColor>
#include <QList>

class Painter : public QWidget
{
    Q_OBJECT
public:
    explicit Painter(int width, int height, QWidget *parent = nullptr);

    QSize sizeHint() const;

    void clear();

    QSize getCanvasSize() const { return canvas.size(); }
    void setCanvasSize(const QSize &size);
    bool saveCanvas(const QString &fileName);

    QColor getPenColor() const { return penColor; }
    void setPenColor(const QColor &color);

    int getCurrentMode() const { return curMode; }
    void setCurrentMode(int mode);

    void setCurrentShape(Shape *shape);

    // enum for curMode
    enum {
        DRAW_LINE_MODE, DRAW_POLYGON_MODE, DRAW_ELLIPSE_MODE,
        DRAW_CURVE_MODE, TRANSFORM_MODE, CLIP_MODE,
    };

signals:
    void shapeAdded(Shape *);
    void currentShapeChanged(Shape *);

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    void paintEventOnDrawLineMode(QPaintEvent *event);
    void mousePressEventOnDrawLineMode(QMouseEvent *event);
    void mouseMoveEventOnDrawLineMode(QMouseEvent *event);
    void mouseReleaseEventOnDrawLineMode(QMouseEvent *event);

    void clearCanvas();
    void addShape(Shape *shape);
    void addShapeAndFocus(Shape *shape);
    void drawShapes();
    void clearShapes();

    // enum for whatIsDoingNow
    enum {
        IDLE, DRAWING_LINE, DRAWING_POLYGON, DRAWING_ELLIPSE,
        DRAWING_CURVE, TRANSFORMING, CLIPPING,
    };

    QImage canvas;
    QColor penColor;
    int curMode;
    int whatIsDoingNow;

    QList<Shape *> shapes;
    Shape *curShape;

    // varibles for drawing lines
    QPoint p1, p2;

};

#endif // PAINTER_H
