#include "mainwindow.h"
#include "paintercli.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    if (argc > 1) {
        PainterCLI cliApp;
        return cliApp.exec(argc, argv);
    }

    QApplication app(argc, argv);
    MainWindow win;
    win.show();

    return app.exec();
}
