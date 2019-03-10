#include "painter.h"
#include "line.h"
#include "utils.h"

#include <QtWidgets>
#include <QtMath>

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

void Painter::paintEvent(QPaintEvent *event)
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

void Painter::mouseReleaseEvent(QMouseEvent *event)
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
        Line(pb, pe, penColor, "").draw(canvas);
    }
}

void Painter::mousePressEventOnDrawLineMode(QMouseEvent *event)
{
    Q_ASSERT(whatIsDoingNow == IDLE);
    if (event->button() == Qt::LeftButton) {
        whatIsDoingNow = DRAWING_LINE;
        pb = event->pos();
    }
}

void Painter::mouseMoveEventOnDrawLineMode(QMouseEvent *event)
{
    if (whatIsDoingNow == DRAWING_LINE) {
        pe = event->pos();
        update();
    }
}

void Painter::mouseReleaseEventOnDrawLineMode(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (whatIsDoingNow == DRAWING_LINE) {
            pe = event->pos();
            if (!Utils::isClose(pb, pe, 10)) {
                addShapeAndFocus(new Line(pb, pe, penColor, ""));
            }
            whatIsDoingNow = IDLE;
            update();
        }
    }
}

void Painter::paintEventOnTransformMode(QPaintEvent * /* event */)
{
    if (curShape) {
        if (whatIsDoingNow != ROTATING) {
            drawRectHull(curShape->getRectHull());
        }
        drawCenter(curShape->getCenter());
    }
}

void Painter::mousePressEventOnTransformMode(QMouseEvent *event)
{
    Q_ASSERT(whatIsDoingNow == IDLE);
    if (event->button() == Qt::LeftButton && curShape) {
        QPoint center = curShape->getCenter();
        QRect hull = curShape->getRectHull();
        QPoint mousePos = event->pos();

        /* Judgement sequence matters! */
        if (inMoveCenterArea(center, mousePos)) {
            whatIsDoingNow = MOVING_CENTER;
            pb = mousePos;
        }
        else if (inScaleArea(hull, mousePos)) {
            whatIsDoingNow = SCALING;
            curShape->beginTransaction();
            pb = mousePos;
            fixedCenter = center;
        }
        else if (inTranslateArea(hull, mousePos)) {
            whatIsDoingNow = TRANSLATING;
            pb = mousePos;
        }
        else if (inRotateArea(center, mousePos)) {
            whatIsDoingNow = ROTATING;
            curShape->beginTransaction();
            pb = mousePos;
            fixedCenter = center;
        }
        else {
            /* Do nothing */
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
        else if (inRotateArea(center, mousePos)) {
            // TODO: replace the method below by setCursor(rotateCursor)
            setCursor(Qt::ArrowCursor);
        }
        else {
            unsetCursor();
        }
    }
    else if (whatIsDoingNow == MOVING_CENTER) {
        curShape->setCenter(mousePos);
        update();
    }
    else if (whatIsDoingNow == SCALING) {
        double s = calculateScale(fixedCenter, pb, mousePos);
        curShape->scale(fixedCenter, s);
        update();
    }
    else if (whatIsDoingNow == TRANSLATING) {
        curShape->translate(mousePos - pb);
        pb = mousePos;
        update();
    }
    else if (whatIsDoingNow == ROTATING) {
        pe = mousePos;
        double r = calculateRotate(fixedCenter, pb, pe);
        curShape->rotate(fixedCenter, r);
        update();
    }
    else {
        Q_ASSERT(false); // Should not reach here
    }
}

void Painter::mouseReleaseEventOnTransformMode(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && curShape) {
        QPoint mousePos = event->pos();

        if (whatIsDoingNow == MOVING_CENTER) {
            curShape->setCenter(mousePos);
        }
        else if (whatIsDoingNow == TRANSLATING) {
            curShape->translate(mousePos - pb);
        }
        else if (whatIsDoingNow == SCALING) {
            double s = calculateScale(fixedCenter, pb, mousePos);
            curShape->scale(fixedCenter, s);
            curShape->commitTransaction();
        }
        else if (whatIsDoingNow == ROTATING) {
            pe = mousePos;
            double r = calculateRotate(fixedCenter, pb, pe);
            curShape->rotate(fixedCenter, r);
        }
        else {
            Q_ASSERT(false); // Should not reach here.
        }
        whatIsDoingNow = IDLE;
        update();
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

double Painter::calculateScale(const QPoint &center, const QPoint &pb, const QPoint &pe)
{
    QPoint vb = pb - center;
    QPoint ve = pe - center;
    return static_cast<double>(Utils::innerProd(vb, ve)) / Utils::innerProd(vb, vb);
}

double Painter::calculateRotate(const QPoint &center, const QPoint &pb, const QPoint &pe)
{
    QPoint vb = pb - center;
    QPoint ve = pe - center;
    double cosTheta = Utils::innerProd(vb, ve) /
            qSqrt(Utils::innerProd(vb, vb) * Utils::innerProd(ve, ve));
    double theta = qAcos(cosTheta);
    if (Utils::crossProd(vb, ve) < 0)
        theta = -theta;
    return theta;
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

bool Painter::inRotateArea(const QPoint & /* center */, const QPoint & /* p */)
{
    return true;
}

bool Painter::inScaleArea(const QRect &hull, const QPoint &p, int radius)
{
    return topLeftScaleArea(hull, radius).contains(p)
            || topRightScaleArea(hull, radius).contains(p)
            || bottomLeftScaleArea(hull, radius).contains(p)
            || bottomRightScaleArea(hull, radius).contains(p);
}

bool Painter::inMoveCenterArea(const QPoint &p, const QPoint &pos, int radius)
{
    return Utils::isClose(p, pos, radius);
}
