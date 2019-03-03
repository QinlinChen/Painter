#include "painter.h"

#include <QtWidgets>

Painter::Painter(int width, int height, QWidget *parent)
    : QWidget(parent), canvas(width, height, QImage::Format_RGB32)
{
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    canvas.fill(Qt::white);
}

QSize Painter::sizeHint() const
{
    return canvas.size();
}

void Painter::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.drawImage(0, 0, canvas);
}

void Painter::setCanvasSize(const QSize &size)
{
    // TODO
    canvas = QImage(size, QImage::Format_RGB32);
    canvas.fill(Qt::white);
    update();
    updateGeometry();
}
