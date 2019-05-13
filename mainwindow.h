#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "shape.h"

#include <QMainWindow>
#include <QIcon>
#include <QColor>
#include <QSize>
#include <QMap>

class QAction;
class QMenu;
class QToolBar;
class QListWidget;
class QListWidgetItem;
class Painter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void adjustCanvasSize();
    void clear();
    void save();
    void setPenColor();
    void drawLine();
    void drawPolygon();
    void drawEllipse();
    void drawCurve();
    void transform();
    void clip();
    void about();

    void addShape(CG::Shape *shape);
    void removeShape(CG::Shape *shape);
    void setCurrentShapeForShapeList(CG::Shape *shape);
    void setCurrentShapeForPainter(QListWidgetItem *current);

private:
    void createActions();
    void createMenus();
    void createDockWindows();
    void createToolBars();

    static QIcon createIconByColor(const QColor &color,
                                   const QSize &size = QSize(32, 32));

    QListWidget *shapeList;
    QMap<CG::Shape *, QListWidgetItem *> shapeManager;

    Painter *painter;

    QMenu *fileMenu;
    QMenu *toolsMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *toolsToolBar;
    QToolBar *editToolBar;
    QAction *adjustCanvasSizeAction;
    QAction *clearAction;
    QAction *saveAction;
    QAction *exitAction;
    QAction *setPenColorAction;
    QAction *drawLineAction;
    QAction *drawPolygonAction;
    QAction *drawEllipseAction;
    QAction *drawCurveAction;
    QAction *transformAction;
    QAction *clipAction;
    QAction *aboutAction;
    QAction *aboutQtAction;
};

#endif // MAINWINDOW_H
