#include "painter.h"
#include "line.h"
#include "polygon.h"
#include "ellipse.h"
#include "curve.h"
#include "utils.h"

#include <QtWidgets>
#include <QtMath>

#include <algorithm>
using std::placeholders::_1;

Painter::Painter(int width, int height, QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    setCanvasSize(QSize(width, height));
    clearCanvas(canvas);
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
    clearCanvas(canvas);
    drawShapes(canvas);

    switch (curMode) {
    case DRAW_LINE_MODE:
        paintEventOnDrawLineMode(event); break;
    case DRAW_POLYGON_MODE:
        paintEventOnDrawPolygonMode(event); break;
    case DRAW_ELLIPSE_MODE:
        paintEventOnDrawEllipseMode(event); break;
    case DRAW_CURVE_MODE:
        paintEventOnDrawCurveMode(event); break;
    case TRANSFORM_MODE:
        paintEventOnTransformMode(event); break;
    case CLIP_MODE:
        paintEventOnClipMode(event); break;
    default:
        qFatal("Should not reach here"); break;
    }

    QPainter painter(this);
    painter.drawImage(0, 0, canvas);
}

void Painter::mousePressEvent(QMouseEvent *event)
{
    switch (curMode) {
    case DRAW_LINE_MODE:
        mousePressEventOnDrawLineMode(event); break;
    case DRAW_POLYGON_MODE:
        mousePressEventOnDrawPolygonMode(event); break;
    case DRAW_ELLIPSE_MODE:
        mousePressEventOnDrawEllipseMode(event); break;
    case DRAW_CURVE_MODE:
        mousePressEventOnDrawCurveMode(event); break;
    case TRANSFORM_MODE:
        mousePressEventOnTransformMode(event); break;
    case CLIP_MODE:
        mousePressEventOnClipMode(event); break;
    default:
        qFatal("Should not reach here"); break;
    }
}

void Painter::mouseMoveEvent(QMouseEvent *event)
{
    switch (curMode) {
    case DRAW_LINE_MODE:
        mouseMoveEventOnDrawLineMode(event); break;
    case DRAW_POLYGON_MODE:
        mouseMoveEventOnDrawPolygonMode(event); break;
    case DRAW_ELLIPSE_MODE:
        mouseMoveEventOnDrawEllipseMode(event); break;
    case DRAW_CURVE_MODE:
        mouseMoveEventOnDrawCurveMode(event); break;
    case TRANSFORM_MODE:
        mouseMoveEventOnTransformMode(event); break;
    case CLIP_MODE:
        mouseMoveEventOnClipMode(event); break;
    default:
        qFatal("Should not reach here"); break;
    }
}

void Painter::mouseReleaseEvent(QMouseEvent *event)
{
    switch (curMode) {
    case DRAW_LINE_MODE:
        mouseReleaseEventOnDrawLineMode(event); break;
    case DRAW_POLYGON_MODE:
        mouseReleaseEventOnDrawPolygonMode(event); break;
    case DRAW_ELLIPSE_MODE:
        mouseReleaseEventOnDrawEllipseMode(event); break;
    case DRAW_CURVE_MODE:
        mouseReleaseEventOnDrawCurveMode(event); break;
    case TRANSFORM_MODE:
        mouseReleaseEventOnTransformMode(event); break;
    case CLIP_MODE:
        mouseReleaseEventOnClipMode(event); break;
    default:
        qFatal("Should not reach here"); break;
    }
}

void Painter::paintEventOnDrawLineMode(QPaintEvent * /* event */)
{
    if (whatIsDoingNow == DRAWING_LINE) {
        cg::Line(pb, pe, penColor, "").draw(canvas);
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
            if (!utils::isClose(pb, pe, 10)) {
                addShapeAndFocus(new cg::Line(pb, pe, penColor, ""));
            }
            whatIsDoingNow = IDLE;
            update();
        }
    }
}

void Painter::paintEventOnDrawPolygonMode(QPaintEvent * /* event */)
{
    if (whatIsDoingNow == DRAWING_POLYGON) {
        Q_ASSERT(points.size() >= 1);
        for (int i = 0; i < points.size() - 1; ++i) {
            cg::Line(points[i], points[i + 1], penColor, "").draw(canvas);
        }
        cg::Line(points.back(), pe, penColor, "").draw(canvas);
    }
}

void Painter::mousePressEventOnDrawPolygonMode(QMouseEvent * /* event */)
{
    /* Do nothing */
}

void Painter::mouseMoveEventOnDrawPolygonMode(QMouseEvent *event)
{
    if (whatIsDoingNow == IDLE)
        return;

    Q_ASSERT(whatIsDoingNow == DRAWING_POLYGON);
    pe = event->pos();
    update();
}

void Painter::mouseReleaseEventOnDrawPolygonMode(QMouseEvent *event)
{
    QPoint mousePos = event->pos();
    if (whatIsDoingNow == IDLE) {
        if (event->button() == Qt::LeftButton) {
            whatIsDoingNow = DRAWING_POLYGON;
            Q_ASSERT(points.empty());
            points.append(mousePos);
        }
    }
    else { /* whatIsDoingNow == DRAWING_POLYGON */
        Q_ASSERT(whatIsDoingNow == DRAWING_POLYGON);
        Q_ASSERT(points.size() > 0);
        if (event->button() == Qt::LeftButton) {
            if (points.size() >= 3 && utils::isClose(mousePos, points.first(), 8)) {
                addShapeAndFocus(new cg::Polygon(points, penColor, ""));
                points.clear();
                whatIsDoingNow = IDLE;
            }
            else {
                if (std::find_if(points.begin(), points.end(),
                                 std::bind(utils::isClose, _1, mousePos, 6))
                        == points.end())
                    points.append(mousePos);
            }
        }
        else if (event->button() == Qt::RightButton) {
            points.removeLast();
            if (points.empty())
                whatIsDoingNow = IDLE;
        }
        else {
            Q_ASSERT(false); /* Should not reach here */
        }
        update();
    }
}

void Painter::paintEventOnDrawEllipseMode(QPaintEvent * /* event */)
{
    if (whatIsDoingNow == DRAWING_ELLIPSE) {
        cg::Ellipse(pb, pe, penColor, "").draw(canvas);
    }
}

void Painter::mousePressEventOnDrawEllipseMode(QMouseEvent *event)
{
    Q_ASSERT(whatIsDoingNow == IDLE);
    if (event->button() == Qt::LeftButton) {
        whatIsDoingNow = DRAWING_ELLIPSE;
        pb = event->pos();
    }
}

void Painter::mouseMoveEventOnDrawEllipseMode(QMouseEvent *event)
{
    if (whatIsDoingNow == DRAWING_ELLIPSE) {
        pe = event->pos();
        update();
    }
}

void Painter::mouseReleaseEventOnDrawEllipseMode(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (whatIsDoingNow == DRAWING_ELLIPSE) {
            pe = event->pos();
            if (!utils::isClose(pb, pe, 10)) {
                addShapeAndFocus(new cg::Ellipse(pb, pe, penColor, ""));
            }
            whatIsDoingNow = IDLE;
            update();
        }
    }
}

void Painter::paintEventOnDrawCurveMode(QPaintEvent * /* event */)
{
    if (whatIsDoingNow == DRAWING_CURVE) {
        Q_ASSERT(points.size() >= 1);
        QVector<QPoint> pointsToDraw = points;
        pointsToDraw.append(pe);

        QPainter painter(&canvas);
        painter.setPen(Qt::DashLine);
        for (int i = 0; i < pointsToDraw.size() - 1; ++i)
            painter.drawLine(pointsToDraw[i], pointsToDraw[i + 1]);
        cg::Curve(pointsToDraw, penColor, "").draw(canvas);
    }
}

void Painter::mousePressEventOnDrawCurveMode(QMouseEvent * /*event*/)
{
    /* Do nothing */
}

void Painter::mouseMoveEventOnDrawCurveMode(QMouseEvent *event)
{
    if (whatIsDoingNow == IDLE)
        return;

    Q_ASSERT(whatIsDoingNow == DRAWING_CURVE);
    pe = event->pos();
    update();
}

void Painter::mouseReleaseEventOnDrawCurveMode(QMouseEvent *event)
{
    QPoint mousePos = event->pos();
    if (whatIsDoingNow == IDLE) {
        if (event->button() == Qt::LeftButton) {
            whatIsDoingNow = DRAWING_CURVE;
            Q_ASSERT(points.empty());
            points.append(mousePos);
        }
    }
    else { /* whatIsDoingNow == DRAWING_CURVE */
        Q_ASSERT(whatIsDoingNow == DRAWING_CURVE);
        Q_ASSERT(points.size() > 0);
        if (event->button() == Qt::LeftButton) {
            if (points.size() >= 2 && utils::isClose(mousePos, points.last(), 8)) {
                addShapeAndFocus(new cg::Curve(points, penColor, ""));
                points.clear();
                whatIsDoingNow = IDLE;
            }
            else {
                points.append(mousePos);
            }
        }
        else if (event->button() == Qt::RightButton) {
            points.removeLast();
            if (points.empty())
                whatIsDoingNow = IDLE;
        }
        else {
            Q_ASSERT(false); /* Should not reach here */
        }
        update();
    }
}

void Painter::paintEventOnTransformMode(QPaintEvent * /* event */)
{
    if (curShape) {
        if (whatIsDoingNow == SCALING) {
            drawRectHull(curShape->getRectHull());
            drawCenter(fixedCenter);
        }
        else if (whatIsDoingNow == ROTATING) {
            double r = calculateRotate(fixedCenter, pb, pe);
            drawRectHull(fixedHull, fixedCenter, r);
            drawCenter(fixedCenter);
        }
        else {
            drawRectHull(curShape->getRectHull());
            drawCenter(curShape->getCenter());
        }
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
            fixedHull = hull;
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
        pe = mousePos;   /* for paint use */
        double r = calculateRotate(fixedCenter, pb, pe);
        curShape->rotate(fixedCenter, r);
        update();
    }
    else {
        Q_ASSERT(false); /* Should not reach here. */
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
            curShape->commitTransaction();
        }
        else {
            Q_ASSERT(false); /* Should not reach here. */
        }
        whatIsDoingNow = IDLE;
        update();
    }
}

void Painter::paintEventOnClipMode(QPaintEvent * /*event*/)
{
    if (whatIsDoingNow == CLIPPING) {
        QPainter painter(&canvas);
        painter.setPen(Qt::DashLine);
        painter.drawRect(QRect(pb, pe));
    }
}

void Painter::mousePressEventOnClipMode(QMouseEvent *event)
{
    Q_ASSERT(whatIsDoingNow == IDLE);
    if (event->button() == Qt::LeftButton) {
        whatIsDoingNow = CLIPPING;
        pb = event->pos();
    }
}

void Painter::mouseMoveEventOnClipMode(QMouseEvent *event)
{
    if (whatIsDoingNow == CLIPPING) {
        pe = event->pos();
        update();
    }
}

void Painter::mouseReleaseEventOnClipMode(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (whatIsDoingNow == CLIPPING) {
            pe = event->pos();
            if (curShape && !utils::isClose(pb, pe, 10)) {
                clipShapeAndRefocus(curShape);
            }
            whatIsDoingNow = IDLE;
            update();
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
    QImage image(canvas.size(), QImage::Format_RGB32);
    clearCanvas(image);
    drawShapes(image);

    return image.save(fileName);
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
        case DRAW_ELLIPSE_MODE:
            setMouseTracking(false);
            setCursor(Qt::CrossCursor);
            break;
        case DRAW_CURVE_MODE:   /* fall through */
        case DRAW_POLYGON_MODE:
            setMouseTracking(true);
            setCursor(Qt::CrossCursor);
            break;
        case TRANSFORM_MODE:
            setMouseTracking(true);
            unsetCursor();
            break;
        case CLIP_MODE:
            setMouseTracking(false);
            // TODO: select a more appropriate cursor
            unsetCursor();
            break;
        default:
            qFatal("Should not reach here");
            break;
        }
        update();
    }
}

void Painter::setCurrentShape(cg::Shape *shape)
{
    if (shape != curShape) {
        curShape = shape;
        emit currentShapeChanged(curShape);
        update();
    }
}

void Painter::clearCanvas(QImage &canvas)
{
    canvas.fill(Qt::white);
}

void Painter::addShape(cg::Shape *shape)
{
    if (!shape)
        return;

    /* It's better to check whether the shape added
     * is already in the shape list. */
    shapes.append(shape);
    emit shapeAdded(shape);
}

void Painter::addShapeAndFocus(cg::Shape *shape)
{
    addShape(shape);
    setCurrentShape(shape);
}

void Painter::removeShape(cg::Shape *shape)
{
    if (!shape)
        return;

    if (!shapes.removeOne(shape)) {
        qDebug("Can't find shape when removing");
        return;
    }
    emit shapeRemoved(shape);
}

void Painter::clipShapeAndRefocus(cg::Shape *shape)
{
    if (!shape)
        return;

    cg::Line *line = dynamic_cast<cg::Line *>(shape);
    if (line) {
        cg::Shape *clippedShape = line->clip(pb, pe, "");
        removeShape(line);
        delete line;
        if (clippedShape)
            addShapeAndFocus(clippedShape);
        else
            setCurrentShape(nullptr);
    }
    else {
        /* Trick: The following code enables you to
         * delete other shapes by clipping. */
        removeShape(shape);
        delete shape;
        setCurrentShape(nullptr);
    }
}

void Painter::drawShapes(QImage &image)
{
    for (auto shape : shapes) {
        shape->draw(image);
    }
}

void Painter::clearShapes()
{
    for (auto shape : shapes) {
        delete shape;
    }
    shapes.clear();
}

void Painter::drawRectHull(const QRect &hull)
{
    QPainter painter(&canvas);
    drawRectHull(&painter, hull);
}

void Painter::drawRectHull(const QRect &hull, const QPoint &center, double theta)
{
    QPainter painter(&canvas);
    painter.translate(center);
    painter.rotate(qRadiansToDegrees(theta));
    QRect rect = hull;
    rect.moveCenter(hull.center() - center);
    drawRectHull(&painter, rect);
}

void Painter::drawRectHull(QPainter *painter, const QRect &hull)
{
    painter->drawRect(hull);
    painter->drawRect(topLeftScaleArea(hull));
    painter->drawRect(topRightScaleArea(hull));
    painter->drawRect(bottomLeftScaleArea(hull));
    painter->drawRect(bottomRightScaleArea(hull));
}

void Painter::drawCenter(const QPoint &p)
{
    QPainter painter(&canvas);
    painter.drawEllipse(p, 4, 4);
    painter.drawPoint(p);
}

double Painter::calculateScale(const QPoint &center,
                               const QPoint &pb, const QPoint &pe)
{
    QPoint vb = pb - center;
    QPoint ve = pe - center;
    return static_cast<double>(utils::innerProd(vb, ve))
            / utils::innerProd(vb, vb);
}

double Painter::calculateRotate(const QPoint &center,
                                const QPoint &pb, const QPoint &pe)
{
    QPoint vb = pb - center;
    QPoint ve = pe - center;
    double be = utils::innerProd(vb, ve);
    double bb = utils::innerProd(vb, vb);
    double ee = utils::innerProd(ve, ve);
    double cosTheta = be / qSqrt(bb * ee);
    double theta = qAcos(cosTheta);
    if (utils::crossProd(vb, ve) < 0)
        theta = -theta;
    return theta;
}

QRect Painter::topLeftScaleArea(const QRect &hull, int radius)
{
    return utils::getRectAroundPoint(hull.topLeft(), radius);
}

QRect Painter::topRightScaleArea(const QRect &hull, int radius)
{
    return utils::getRectAroundPoint(hull.topRight(), radius);
}

QRect Painter::bottomLeftScaleArea(const QRect &hull, int radius)
{
    return utils::getRectAroundPoint(hull.bottomLeft(), radius);
}

QRect Painter::bottomRightScaleArea(const QRect &hull, int radius)
{
    return utils::getRectAroundPoint(hull.bottomRight(), radius);
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
    return utils::isClose(p, pos, radius);
}
