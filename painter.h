#ifndef PAINTER_H
#define PAINTER_H

#include "shape.h"

#include <QWidget>
#include <QImage>
#include <QSize>
#include <QPoint>
#include <QColor>
#include <QList>
#include <QVector>

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

    void setCurrentShape(CG::Shape *shape);

    // enum for curMode
    enum {
        DRAW_LINE_MODE, DRAW_POLYGON_MODE, DRAW_ELLIPSE_MODE,
        DRAW_CURVE_MODE, TRANSFORM_MODE, CLIP_MODE,
    };

signals:
    void shapeAdded(CG::Shape *);
    void currentShapeChanged(CG::Shape *);

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

    void paintEventOnDrawPolygonMode(QPaintEvent *event);
    void mousePressEventOnDrawPolygonMode(QMouseEvent *event);
    void mouseMoveEventOnDrawPolygonMode(QMouseEvent *event);
    void mouseReleaseEventOnDrawPolygonMode(QMouseEvent *event);

    void paintEventOnDrawEllipseMode(QPaintEvent *event);
    void mousePressEventOnDrawEllipseMode(QMouseEvent *event);
    void mouseMoveEventOnDrawEllipseMode(QMouseEvent *event);
    void mouseReleaseEventOnDrawEllipseMode(QMouseEvent *event);

    void paintEventOnDrawCurveMode(QPaintEvent *event);
    void mousePressEventOnDrawCurveMode(QMouseEvent *event);
    void mouseMoveEventOnDrawCurveMode(QMouseEvent *event);
    void mouseReleaseEventOnDrawCurveMode(QMouseEvent *event);

    void paintEventOnTransformMode(QPaintEvent *event);
    void mousePressEventOnTransformMode(QMouseEvent *event);
    void mouseMoveEventOnTransformMode(QMouseEvent *event);
    void mouseReleaseEventOnTransformMode(QMouseEvent *event);

    void paintEventOnClipMode(QPaintEvent *event);
    void mousePressEventOnClipMode(QMouseEvent *event);
    void mouseMoveEventOnClipMode(QMouseEvent *event);
    void mouseReleaseEventOnClipMode(QMouseEvent *event);

    void clearCanvas();
    void addShape(CG::Shape *shape);
    void addShapeAndFocus(CG::Shape *shape);
    void drawShapes();
    void clearShapes();

    void drawRectHull(const QRect &hull);
    void drawRectHull(const QRect &hull, const QPoint &center, double theta);
    void drawRectHull(QPainter *painter, const QRect &hull);
    void drawCenter(const QPoint &p);

    static double calculateScale(const QPoint &fixedCenter,
                                 const QPoint &pb, const QPoint &pe);
    static double calculateRotate(const QPoint &fixedCenter,
                                  const QPoint &pb, const QPoint &pe);

    static QRect topLeftScaleArea(const QRect &hull, int radius = 4);
    static QRect topRightScaleArea(const QRect &hull, int radius = 4);
    static QRect bottomLeftScaleArea(const QRect &hull, int radius = 4);
    static QRect bottomRightScaleArea(const QRect &hull, int radius = 4);
    static bool inMoveCenterArea(const QPoint &p, const QPoint &pos, int radius = 4);
    static bool inScaleArea(const QRect &hull, const QPoint &p, int radius = 4);
    static bool inTranslateArea(const QRect &hull, const QPoint &p);
    static bool inRotateArea(const QPoint &fixedCenter, const QPoint &p);

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

    QList<CG::Shape *> shapes;
    CG::Shape *curShape;

    /* temporary varibles for drawing lines, transfroming, etc. */
    QPoint pb, pe;      /* pointBegin, pointEnd */
    QPoint fixedCenter; /* Remember the center when scaling and rotating. */
    QRect fixedHull;    /* Remember the hull when rotating. */
    QVector<QPoint> points;   /* for drawing polygons */
};

#endif // PAINTER_H
