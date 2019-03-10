#include "painter.h"
#include "line.h"
#include "utils.h"

#include <QtWidgets>

Painter::Painter(int width, int height, QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    setCanvasSize(QSize(width, height));
    clearCanvas();
    setPenColor(Qt::black);
    setCurrentMode(DRAW_LINE_MODE);
    setCurrentShape(nullptr);
    whatIsDoingNow = IDLE;
}

QSize Painter::sizeHint() const
{
    return canvas.size();
}

void Painter::clear()
{
    setCurrentShape(nullptr);
    clearShapes();
    update();
}

void Painter::paintEvent(QPaintEvent * event)
{
    clearCanvas();
    drawShapes();

    switch (curMode) {
    case DRAW_LINE_MODE:
        paintEventOnDrawLineMode(event); break;
    case DRAW_POLYGON_MODE:
        qDebug("paintEventOnDrawPolygonMode(event)"); break;
    case DRAW_ELLIPSE_MODE:
        qDebug("paintEventOnDrawEllipseMode(event)"); break;
    case DRAW_CURVE_MODE:
        qDebug("paintEventOnDrawCurveMode(event)"); break;
    case TRANSFORM_MODE:
        paintEventOnTransformMode(event); break;
    case CLIP_MODE:
        qDebug("paintEventOnClipMode(event)"); break;
    default:
        qFatal("Should not reach here"); break;
    }

    QPainter painter(this);
    painter.drawImage(0, 0, canvas);
}

void Painter::mousePressEvent(QMouseEvent *event)
{
    // TODO
    switch (curMode) {
    case DRAW_LINE_MODE:
        mousePressEventOnDrawLineMode(event); break;
    case DRAW_POLYGON_MODE:
        qDebug("mousePressEventOnDrawPolygonMode(event)"); break;
    case DRAW_ELLIPSE_MODE:
        qDebug("mousePressEventOnDrawEllipseMode(event)"); break;
    case DRAW_CURVE_MODE:
        qDebug("mousePressEventOnDrawCurveMode(event)"); break;
    case TRANSFORM_MODE:
        mousePressEventOnTransformMode(event); break;
    case CLIP_MODE:
        qDebug("mousePressEventOnClipMode(event)"); break;
    default:
        qFatal("Should not reach here"); break;
    }
}

void Painter::mouseMoveEvent(QMouseEvent *event)
{
    // TODO
    switch (curMode) {
    case DRAW_LINE_MODE:
        mouseMoveEventOnDrawLineMode(event); break;
    case DRAW_POLYGON_MODE:
        qDebug("mouseMoveEventOnDrawPolygonMode(event)"); break;
    case DRAW_ELLIPSE_MODE:
        qDebug("mouseMoveEventOnDrawEllipseMode(event)"); break;
    case DRAW_CURVE_MODE:
        qDebug("mouseMoveEventOnDrawCurveMode(event)"); break;
    case TRANSFORM_MODE:
        mouseMoveEventOnTransformMode(event); break;
    case CLIP_MODE:
        qDebug("mouseMoveEventOnClipMode(event)"); break;
    default:
        qFatal("Should not reach here"); break;
    }
}

void Painter::mouseReleaseEvent(QMouseEvent * event)
{
    // TODO
    switch (curMode) {
    case DRAW_LINE_MODE:
        mouseReleaseEventOnDrawLineMode(event); break;
    case DRAW_POLYGON_MODE:
        qDebug("mouseReleaseEventOnDrawPolygonMode(event)"); break;
    case DRAW_ELLIPSE_MODE:
        qDebug("mouseReleaseEventOnDrawEllipseMode(event)"); break;
    case DRAW_CURVE_MODE:
        qDebug("mouseReleaseEventOnDrawCurveMode(event)"); break;
    case TRANSFORM_MODE:
        mouseReleaseEventOnTransformMode(event); break;
    case CLIP_MODE:
        qDebug("mouseReleaseEventOnClipMode(event)"); break;
    default:
        qFatal("Should not reach here"); break;
    }
}

void Painter::paintEventOnDrawLineMode(QPaintEvent * /* event */)
{
    if (whatIsDoingNow == DRAWING_LINE) {
        Line(p1, p2, penColor, "").draw(canvas);
    }
}

void Painter::mousePressEventOnDrawLineMode(QMouseEvent *event)
{
    Q_ASSERT(whatIsDoingNow == IDLE);
    if (event->button() == Qt::LeftButton) {
        whatIsDoingNow = DRAWING_LINE;
        p1 = event->pos();
    }
}

void Painter::mouseMoveEventOnDrawLineMode(QMouseEvent *event)
{
    if (whatIsDoingNow == DRAWING_LINE) {
        p2 = event->pos();
        update();
    }
}

void Painter::mouseReleaseEventOnDrawLineMode(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (whatIsDoingNow == DRAWING_LINE) {
            whatIsDoingNow = IDLE;
            if (!Utils::isClose(p1, event->pos(), 10)) {
                addShapeAndFocus(new Line(p1, event->pos(), penColor, ""));
            }
            update();
        }
    }
}

void Painter::paintEventOnTransformMode(QPaintEvent * /* event */)
{
    if (curShape) {
        drawRectHull(curShape->getRectHull());
        drawCenter(curShape->getCenter());
    }
}

void Painter::mousePressEventOnTransformMode(QMouseEvent *event)
{
    Q_ASSERT(whatIsDoingNow == IDLE);
    if (event->button() == Qt::LeftButton && curShape) {
        QRect hull = curShape->getRectHull();
        QPoint center = curShape->getCenter();
        QPoint mousePos = event->pos();
        /* Judgement sequence matters! */
        if (inMoveCenterArea(center, mousePos)) {
            whatIsDoingNow = MOVING_CENTER;
            p1 = mousePos;
        }
        else if (inScaleArea(hull, mousePos)) {
            whatIsDoingNow = SCALING;
            // TODO
            qDebug("begin scaling");
        }
        else if (inTranslateArea(hull, mousePos)) {
            whatIsDoingNow = TRANSLATING;
            p1 = mousePos;
        }
        else if (inRotateArea(hull, mousePos)) {
            whatIsDoingNow = ROTATING;
            // TODO
            qDebug("begin rotating");
        }
        else {
            // Do nothing
        }
    }
}

void Painter::mouseMoveEventOnTransformMode(QMouseEvent *event)
{
    if (!curShape)
        return;

    QPoint mousePos = event->pos();
    if (whatIsDoingNow == IDLE) {
        QRect hull = curShape->getRectHull();
        QPoint center = curShape->getCenter();
        if (inMoveCenterArea(center, mousePos)) {
            setCursor(Qt::ArrowCursor);
        }
        else if (topLeftScaleArea(hull).contains(mousePos)
                  || bottomRightScaleArea(hull).contains(mousePos)) {
            setCursor(Qt::SizeFDiagCursor);
        }
        else if (topRightScaleArea(hull).contains(mousePos)
                 || bottomLeftScaleArea(hull).contains(mousePos)) {
            setCursor(Qt::SizeBDiagCursor);
        }
        else if (inTranslateArea(hull, mousePos)) {
            setCursor(Qt::SizeAllCursor);
        }
        else if (inRotateArea(hull, mousePos)) {
            // TODO: replace the method below by setCursor(rotateCursor)
            setCursor(Qt::OpenHandCursor);
        }
        else {
            unsetCursor();
        }
    }
    else if (whatIsDoingNow == MOVING_CENTER) {
        curShape->setCenter(mousePos.x(), mousePos.y());
        update();
    }
    else if (whatIsDoingNow == SCALING) {
        // TODO
        qDebug("scaling");
    }
    else if (whatIsDoingNow == TRANSLATING) {
        p2 = mousePos;
        curShape->translate(p2.x() - p1.x(), p2.y() - p1.y());
        p1 = p2;
        update();
    }
    else if (whatIsDoingNow == ROTATING) {
        // TODO
        qDebug("rotating");
    }
    else {
        Q_ASSERT(false); // Should not reach here
    }
}

void Painter::mouseReleaseEventOnTransformMode(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint mousePos = event->pos();
        if (whatIsDoingNow == MOVING_CENTER) {
            whatIsDoingNow = IDLE;
            curShape->setCenter(mousePos.x(), mousePos.y());
            update();
        }
        else if (whatIsDoingNow == TRANSLATING) {
            whatIsDoingNow = IDLE;
            p2 = mousePos;
            curShape->translate(p2.x() - p1.x(), p2.y() - p1.y());
            update();
        }
        else if (whatIsDoingNow == ROTATING) {
            whatIsDoingNow = IDLE;
            // TODO
            qDebug("stop rotating");
        }
        else if (whatIsDoingNow == SCALING) {
            whatIsDoingNow = IDLE;
            qDebug("stop scaling");
        }
        else {
            Q_ASSERT(false); // Should not reach here.
        }
    }
}

void Painter::setCanvasSize(const QSize &size)
{
    if (size != canvas.size()) {
        canvas = QImage(size, QImage::Format_RGB32);
        update();
        updateGeometry();
    }
}

bool Painter::saveCanvas(const QString &fileName)
{
    return canvas.save(fileName);
}

void Painter::setPenColor(const QColor &color)
{
    penColor = color;
}

void Painter::setCurrentMode(int mode)
{
    if (mode != curMode) {
        curMode = mode;
        switch (curMode) {
        case DRAW_LINE_MODE:    /* fall through */
        case DRAW_POLYGON_MODE: /* fall through */
        case DRAW_ELLIPSE_MODE: /* fall through */
        case DRAW_CURVE_MODE:
            setMouseTracking(false);
            setCursor(Qt::CrossCursor);
            break;
        case TRANSFORM_MODE:
            setMouseTracking(true);
            unsetCursor();
            break;
        case CLIP_MODE:
            // TODO
            setMouseTracking(false);
            unsetCursor();
            break;
        default:
            qFatal("Should not reach here"); break;
        }
        update();
    }
}

void Painter::setCurrentShape(Shape *shape)
{
    if (shape != curShape) {
        curShape = shape;
        emit currentShapeChanged(curShape);
        update();
    }
}

void Painter::clearCanvas()
{
    canvas.fill(Qt::white);
}

void Painter::addShape(Shape *shape)
{
    if (shape) {
        /* It's better to check whether the shape added
         * is already in the shape list. */
        shapes.append(shape);
        emit shapeAdded(shape);
    }
}

void Painter::addShapeAndFocus(Shape *shape)
{
    addShape(shape);
    setCurrentShape(shape);
}

void Painter::drawShapes()
{
    foreach(Shape *shape, shapes) {
        shape->draw(canvas);
    }
}

void Painter::clearShapes()
{
    foreach(Shape *shape, shapes) {
        delete shape;
    }
    shapes.clear();
}

void Painter::drawRectHull(const QRect &hull)
{
    QPainter painter(&canvas);
    painter.drawRect(hull);
    painter.drawRect(topLeftScaleArea(hull));
    painter.drawRect(topRightScaleArea(hull));
    painter.drawRect(bottomLeftScaleArea(hull));
    painter.drawRect(bottomRightScaleArea(hull));
}

void Painter::drawCenter(const QPoint &p)
{
    QPainter painter(&canvas);
    painter.drawEllipse(p, 4, 4);
    painter.drawPoint(p);
}

QRect Painter::topLeftScaleArea(const QRect &hull, int radius)
{
    return Utils::getRectAroundPoint(hull.topLeft(), radius);
}

QRect Painter::topRightScaleArea(const QRect &hull, int radius)
{
    return Utils::getRectAroundPoint(hull.topRight(), radius);
}

QRect Painter::bottomLeftScaleArea(const QRect &hull, int radius)
{
    return Utils::getRectAroundPoint(hull.bottomLeft(), radius);
}

QRect Painter::bottomRightScaleArea(const QRect &hull, int radius)
{
    return Utils::getRectAroundPoint(hull.bottomRight(), radius);
}

bool Painter::inTranslateArea(const QRect &hull, const QPoint &p)
{
    return hull.contains(p);
}

bool Painter::inRotateArea(const QRect &hull, const QPoint &p)
{
    return !hull.contains(p);
}

bool Painter::inScaleArea(const QRect &hull, const QPoint &p, int radius)
{
    return topLeftScaleArea(hull).contains(p, radius)
            || topRightScaleArea(hull).contains(p, radius)
            || bottomLeftScaleArea(hull).contains(p, radius)
            || bottomRightScaleArea(hull).contains(p, radius);
}

bool Painter::inMoveCenterArea(const QPoint &p, const QPoint &pos, int radius)
{
    return Utils::isClose(p, pos, radius);
}
