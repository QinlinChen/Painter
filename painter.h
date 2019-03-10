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

    void paintEventOnTransformMode(QPaintEvent *event);
    void mousePressEventOnTransformMode(QMouseEvent *event);
    void mouseMoveEventOnTransformMode(QMouseEvent *event);
    void mouseReleaseEventOnTransformMode(QMouseEvent *event);

    void clearCanvas();
    void addShape(Shape *shape);
    void addShapeAndFocus(Shape *shape);
    void drawShapes();
    void clearShapes();
    void drawRectHull(const QRect &hull);
    void drawCenter(const QPoint &p);

    static QRect topLeftScaleArea(const QRect &hull, int radius = 4);
    static QRect topRightScaleArea(const QRect &hull, int radius = 4);
    static QRect bottomLeftScaleArea(const QRect &hull, int radius = 4);
    static QRect bottomRightScaleArea(const QRect &hull, int radius = 4);
    static bool inTranslateArea(const QRect &hull, const QPoint &p);
    static bool inRotateArea(const QRect &hull, const QPoint &p);
    static bool inScaleArea(const QRect &hull, const QPoint &p, int radius = 4);
    static bool inMoveCenterArea(const QPoint &p, const QPoint &pos, int radius = 4);

    // enum for whatIsDoingNow
    enum {
        IDLE,
        DRAWING_LINE, DRAWING_POLYGON,
        DRAWING_ELLIPSE, DRAWING_CURVE,
        MOVING_CENTER, TRANSLATING, ROTATING, SCALING,
        CLIPPING,
    };

    QImage canvas;
    QColor penColor;
    int curMode;
    int whatIsDoingNow;

    QList<Shape *> shapes;
    Shape *curShape;

    // varibles for drawing lines, translating, moving center
    QPoint p1, p2;

};

#endif // PAINTER_H
