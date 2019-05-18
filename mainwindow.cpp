#include "mainwindow.h"
#include "painter.h"
#include "qsizedialog.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    painter = new Painter(800, 600);
    setCentralWidget(painter);
    connect(painter, SIGNAL(shapeAdded(cg::Shape *)),
            this, SLOT(addShape(cg::Shape *)));
    connect(painter, SIGNAL(shapeRemoved(cg::Shape *)),
            this, SLOT(removeShape(cg::Shape *)));
    connect(painter, SIGNAL(currentShapeChanged(cg::Shape *)),
            this, SLOT(setCurrentShapeForShapeList(cg::Shape *)));

    createActions();
    createMenus();
    createDockWindows();
    createToolBars();

    setWindowTitle(tr("Painter"));
    setWindowIcon(QIcon(":/images/painter.png"));
}

MainWindow::~MainWindow()
{

}

void MainWindow::createActions()
{
    adjustCanvasSizeAction = new QAction(tr("Canvas Size"), this);
    adjustCanvasSizeAction->setShortcut(tr("Ctrl+R"));
    adjustCanvasSizeAction->setStatusTip(tr("Adjust the canvas size"));
    connect(adjustCanvasSizeAction, SIGNAL(triggered()),
            this, SLOT(adjustCanvasSize()));

    clearAction = new QAction(tr("Clear"), this);
    clearAction->setIcon(QIcon(":/images/reset.png"));
    clearAction->setStatusTip(tr("Clear the canvas"));
    connect(clearAction, SIGNAL(triggered()),
            this, SLOT(clear()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setIcon(QIcon(":/images/save.png"));
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save the canvas as a picture"));
    connect(saveAction, SIGNAL(triggered()),
            this, SLOT(save()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setIcon(QIcon(":/images/exit.png"));
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()),
            this, SLOT(close()));

    setPenColorAction = new QAction(tr("Pen Color"));
    setPenColorAction->setIcon(createIconByColor(painter->getPenColor()));
    setPenColorAction->setStatusTip(tr("Set the pen color"));
    connect(setPenColorAction, SIGNAL(triggered()),
            this, SLOT(setPenColor()));

    drawLineAction = new QAction(tr("Line"));
    drawLineAction->setIcon(QIcon(":/images/line.png"));
    drawLineAction->setStatusTip(tr("Draw lines"));
    connect(drawLineAction, SIGNAL(triggered()),
            this, SLOT(drawLine()));

    drawPolygonAction = new QAction(tr("Polygon"));
    drawPolygonAction->setIcon(QIcon(":/images/polygon.png"));
    drawPolygonAction->setStatusTip(tr("Draw polygons"));
    connect(drawPolygonAction, SIGNAL(triggered()),
            this, SLOT(drawPolygon()));

    drawEllipseAction = new QAction(tr("Ellipse"));
    drawEllipseAction->setIcon(QIcon(":/images/ellipse.png"));
    drawEllipseAction->setStatusTip(tr("Draw ellipses"));
    connect(drawEllipseAction, SIGNAL(triggered()),
            this, SLOT(drawEllipse()));

    drawCurveAction = new QAction(tr("Curve"));
    drawCurveAction->setIcon(QIcon(":/images/curve.png"));
    drawCurveAction->setStatusTip(tr("Draw curves"));
    connect(drawCurveAction, SIGNAL(triggered()),
            this, SLOT(drawCurve()));

    transformAction = new QAction(tr("Transform"), this);
    transformAction->setIcon(QIcon(":/images/transform.png"));
    transformAction->setShortcut(tr("Ctrl+T"));
    transformAction->setStatusTip(tr("Transform the current shape"));
    connect(transformAction, SIGNAL(triggered()),
            this, SLOT(transform()));

    clipAction = new QAction(tr("Clip"), this);
    clipAction->setIcon(QIcon(":/images/clip.png"));
    clipAction->setStatusTip(tr("Clip the current shape"));
    connect(clipAction, SIGNAL(triggered()),
            this, SLOT(clip()));

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("Show the application's About box"));
    connect(aboutAction, SIGNAL(triggered()),
            this, SLOT(about()));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAction, SIGNAL(triggered()),
            qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(adjustCanvasSizeAction);
    fileMenu->addAction(clearAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(setPenColorAction);
    toolsMenu->addSeparator();
    toolsMenu->addAction(drawLineAction);
    toolsMenu->addAction(drawPolygonAction);
    toolsMenu->addAction(drawEllipseAction);
    toolsMenu->addAction(drawCurveAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(transformAction);
    editMenu->addAction(clipAction);

    viewMenu = menuBar()->addMenu(tr("&View"));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}

void MainWindow::createDockWindows()
{
    QDockWidget *dock = new QDockWidget(tr("Shapes"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    shapeList = new QListWidget(dock);
    connect(shapeList, SIGNAL(currentItemChanged(QListWidgetItem *,
                                                 QListWidgetItem *)),
            this, SLOT(setCurrentShapeForPainter(QListWidgetItem *)));
    dock->setWidget(shapeList);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(clearAction);
    fileToolBar->addAction(saveAction);

    toolsToolBar = addToolBar(tr("&Tools"));
    toolsToolBar->addAction(setPenColorAction);
    toolsToolBar->addAction(drawLineAction);
    toolsToolBar->addAction(drawPolygonAction);
    toolsToolBar->addAction(drawEllipseAction);
    toolsToolBar->addAction(drawCurveAction);

    editToolBar = addToolBar(tr("&Edit"));
    editToolBar->addAction(transformAction);
    editToolBar->addAction(clipAction);
}

QIcon MainWindow::createIconByColor(const QColor &color, const QSize &size)
{
    QPixmap pixmap(size);
    pixmap.fill(color);
    return QIcon(pixmap);
}

void MainWindow::adjustCanvasSize()
{
    QSize size = QSizeDialog::getSize(QSize(4096, 2160),
                                      painter->getCanvasSize(),
                                      this, tr("Reset Canvas"));
    if (!size.isEmpty()) {
        painter->setCanvasSize(size);
    }
}

void MainWindow::clear()
{
    shapeManager.clear();
    shapeList->clear();
    painter->clear();
}

void MainWindow::save()
{
    QString fileName = QFileDialog::getSaveFileName(
                this, tr("Save File"), "untitled",
                tr("BMP (*.bmp);;JPEG (*.jpg);;PNG (*.png)"));
    if (!fileName.isEmpty()) {
        painter->saveCanvas(fileName);
    }
}

void MainWindow::setPenColor()
{
    QColor color = QColorDialog::getColor(painter->getPenColor(),
                                          this, tr("Select Pen Color"));
    if (color.isValid()) {
        painter->setPenColor(color);
        setPenColorAction->setIcon(createIconByColor(color));
    }
}

void MainWindow::drawLine()
{
    painter->setCurrentMode(Painter::DRAW_LINE_MODE);
}

void MainWindow::drawPolygon()
{
    painter->setCurrentMode(Painter::DRAW_POLYGON_MODE);
}

void MainWindow::drawEllipse()
{
    painter->setCurrentMode(Painter::DRAW_ELLIPSE_MODE);
}

void MainWindow::drawCurve()
{
    painter->setCurrentMode(Painter::DRAW_CURVE_MODE);
}

void MainWindow::transform()
{
    painter->setCurrentMode(Painter::TRANSFORM_MODE);
}

void MainWindow::clip()
{
    painter->setCurrentMode(Painter::CLIP_MODE);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Painter"),
            tr("<h2>Painter 0.1</h2>"
               "<p>Copyright &copy; 2019 Qinlin Chen."
               "<p>Painter is a small application that "
               "can paint shapes and do transformations "
               "on them."));
}

void MainWindow::addShape(cg::Shape *shape)
{
    static int id = 1;
    if (!shape) {
        qDebug("addShape(nullptr)");
        return;
    }

    QString text = shape->shapeName() + " " + QString::number(id++);
    QListWidgetItem *item = new QListWidgetItem(text);
    QVariant data = QVariant::fromValue(static_cast<void *>(shape));
    item->setData(Qt::UserRole, data);

    shapeList->insertItem(0, item);
    shapeManager.insert(shape, item);
}

void MainWindow::removeShape(cg::Shape *shape)
{
    if (!shape) {
        qDebug("removeShape(nullptr)");
        return;
    }

    QListWidgetItem *item = shapeManager.take(shape);
    if (!item) {
        qDebug("Can't find shape to remove!");
        return;
    }
    shapeList->removeItemWidget(item);
    delete item;
}

void MainWindow::setCurrentShapeForShapeList(cg::Shape *shape)
{
    QListWidgetItem *item = nullptr;
    if (shape) {
       item = shapeManager.value(shape, nullptr);
       if (!item) {
           qDebug("List widget item not found");
           return;
       }
    }
    shapeList->setCurrentItem(item);
}

void MainWindow::setCurrentShapeForPainter(QListWidgetItem *current)
{
    cg::Shape *shape = nullptr;
    if (current) {
        QVariant data = current->data(Qt::UserRole);
        shape = static_cast<cg::Shape *>(data.value<void *>());
    }
    painter->setCurrentShape(shape);
}

