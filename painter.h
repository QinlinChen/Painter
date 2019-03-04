#ifndef PAINTER_H
#define PAINTER_H

#include <QWidget>
#include <QImage>

class Painter : public QWidget
{
    Q_OBJECT
public:
    explicit Painter(int width, int height, QWidget *parent = nullptr);

    QSize sizeHint() const;

    QSize getCanvasSize() const { return canvas.size(); }
    void setCanvasSize(const QSize &size);
    bool saveCanvas(const QString &fileName);

    QColor getPenColor() const { return penColor; }
    void setPenColor(const QColor &color);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);

private:
    QImage canvas;
    QColor penColor;
};

#endif // PAINTER_H
