#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QMenu;
class QToolBar;
class QListWidget;
class Painter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void save();
    void setPenColor();
    void drawLine();
    void drawPolygon();
    void drawEllipse();
    void drawCurve();
    void resetCanvas();
    void transform();
    void clip();
    void about();

private:
    void createActions();
    void createMenus();
    void createDockWindows();
    void createContextMenu();
    void createToolBars();

    static QIcon createIconByColor(const QColor &color,
                                   const QSize &size = QSize(32, 32));

    QListWidget *shapeList;
    Painter *painter;

    QMenu *fileMenu;
    QMenu *toolsMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *toolsToolBar;
    QToolBar *editToolBar;
    QAction *resetCanvasAction;
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
