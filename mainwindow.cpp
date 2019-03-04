#include "mainwindow.h"
#include "painter.h"
#include "qsizedialog.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    painter = new Painter(400, 300);
    setCentralWidget(painter);

    createActions();
    createMenus();
    createDockWindows();
    createContextMenu();
    createToolBars();

    setWindowTitle(tr("Painter"));
    setWindowIcon(QIcon(":/images/painter.png"));
}

MainWindow::~MainWindow()
{

}

void MainWindow::createActions()
{
    resetCanvasAction = new QAction(tr("Reset"), this);
    resetCanvasAction->setIcon(QIcon(":/images/reset.png"));
    resetCanvasAction->setShortcut(tr("Ctrl+R"));
    resetCanvasAction->setStatusTip(tr("Reset the canvas size"));
    connect(resetCanvasAction, SIGNAL(triggered()), this, SLOT(resetCanvas()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setIcon(QIcon(":/images/save.png"));
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save the canvas as a picture"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setIcon(QIcon(":/images/exit.png"));
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    setPenColorAction = new QAction(tr("Pen Color"));
    setPenColorAction->setIcon(createIconByColor(painter->getPenColor()));
    setPenColorAction->setStatusTip(tr("Set the pen color"));
    connect(setPenColorAction, SIGNAL(triggered()), this, SLOT(setPenColor()));

    drawLineAction = new QAction(tr("Line"));
    drawLineAction->setIcon(QIcon(":/images/line.png"));
    drawLineAction->setStatusTip(tr("Draw lines"));
    connect(drawLineAction, SIGNAL(triggered()), this, SLOT(drawLine()));

    drawPolygonAction = new QAction(tr("Polygon"));
    drawPolygonAction->setIcon(QIcon(":/images/polygon.png"));
    drawPolygonAction->setStatusTip(tr("Draw polygons"));
    connect(drawPolygonAction, SIGNAL(triggered()), this, SLOT(drawPolygon()));

    drawEllipseAction = new QAction(tr("Ellipse"));
    drawEllipseAction->setIcon(QIcon(":/images/ellipse.png"));
    drawEllipseAction->setStatusTip(tr("Draw ellipses"));
    connect(drawEllipseAction, SIGNAL(triggered()), this, SLOT(drawEllipse()));

    drawCurveAction = new QAction(tr("Curve"));
    drawCurveAction->setIcon(QIcon(":/images/curve.png"));
    drawCurveAction->setStatusTip(tr("Draw curves"));
    connect(drawCurveAction, SIGNAL(triggered()), this, SLOT(drawCurve()));

    transformAction = new QAction(tr("Transform"), this);
    transformAction->setIcon(QIcon(":/images/transform.png"));
    transformAction->setShortcut(tr("Ctrl+T"));
    transformAction->setStatusTip(tr("Transform the current shape"));
    connect(transformAction, SIGNAL(triggered()), this, SLOT(transform()));

    clipAction = new QAction(tr("Clip"), this);
    clipAction->setIcon(QIcon(":/images/clip.png"));
    clipAction->setStatusTip(tr("Clip the current shape"));
    connect(clipAction, SIGNAL(triggered()), this, SLOT(clip()));

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("Show the application's About box"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(resetCanvasAction);
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
    shapeList->addItems(QStringList()
                        << "Line 1"
                        << "Curve 2"
                        << "Polygon 3");
    dock->setWidget(shapeList);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());
}

void MainWindow::createContextMenu()
{
    painter->addAction(drawLineAction);
    painter->addAction(drawPolygonAction);
    painter->addAction(drawEllipseAction);
    painter->addAction(drawCurveAction);
    painter->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(resetCanvasAction);
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

void MainWindow::save()
{
    // TODO
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
    // TODO
    qDebug() << "drawLine()" << endl;
}

void MainWindow::drawPolygon()
{
    // TODO
    qDebug() << "drawPolygon()" << endl;
}

void MainWindow::drawEllipse()
{
    // TODO
    qDebug() << "drawEllipse()" << endl;
}

void MainWindow::drawCurve()
{
    // TODO
    qDebug() << "drawCurve()" << endl;
}

void MainWindow::resetCanvas()
{
    QSize size = QSizeDialog::getSize(QSize(4096, 2160),
                                      painter->getCanvasSize(),
                                      this, tr("Reset Canvas"));
    if (!size.isEmpty()) {
        painter->setCanvasSize(size);
    }
}

void MainWindow::transform()
{
    // TODO
    qDebug() << "transform()" << endl;
}

void MainWindow::clip()
{
    // TODO
    qDebug() << "clip()" << endl;
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

