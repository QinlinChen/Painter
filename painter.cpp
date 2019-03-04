#include "painter.h"
#include "line.h"

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
    whatIsDoingNow = IDLE;
}

QSize Painter::sizeHint() const
{
    return canvas.size();
}

void Painter::paintEvent(QPaintEvent * event)
{
    clearCanvas();
    drawShapes();

    switch (curMode) {
    case DRAW_LINE_MODE:
        paintEventOnDrawLineMode(event); break;
    case DRAW_POLYGON_MODE: break;
    case DRAW_ELLIPSE_MODE: break;
    case DRAW_CURVE_MODE: break;
    case TRANSFORM_MODE: break;
    case CLIP_MODE: break;
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
    case DRAW_POLYGON_MODE: break;
    case DRAW_ELLIPSE_MODE: break;
    case DRAW_CURVE_MODE: break;
    case TRANSFORM_MODE: break;
    case CLIP_MODE: break;
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
    case DRAW_POLYGON_MODE: break;
    case DRAW_ELLIPSE_MODE: break;
    case DRAW_CURVE_MODE: break;
    case TRANSFORM_MODE: break;
    case CLIP_MODE: break;
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
    case DRAW_POLYGON_MODE: break;
    case DRAW_ELLIPSE_MODE: break;
    case DRAW_CURVE_MODE: break;
    case TRANSFORM_MODE: break;
    case CLIP_MODE: break;
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
    if (event->button() == Qt::LeftButton
            && whatIsDoingNow == DRAWING_LINE) {
        whatIsDoingNow = IDLE;
        addShape(new Line(p1, event->pos(), penColor, ""));
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
            setCursor(Qt::CrossCursor); break;
        case TRANSFORM_MODE: /* fall through */
        case CLIP_MODE:
            unsetCursor(); break;
        default:
            qFatal("Should not reach here"); break;
        }
    }
}

void Painter::clearCanvas()
{
    canvas.fill(Qt::white);
}

void Painter::addShape(Shape *shape)
{
    shapes.append(shape);
    emit shapeAdded(shape);
}

void Painter::drawShapes()
{
    foreach(Shape *shape, shapes) {
        shape->draw(canvas);
    }
}
