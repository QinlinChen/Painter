#include "paintercli.h"

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QtDebug>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

int PainterCLI::exec(int argc, char *argv[])
{
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <inFile> <outDir>" << endl;
        return 1;
    }

    /* initialize */
    QFile inFile(argv[1]);
    if (!inFile.open(QIODevice::ReadOnly)) {
        cerr << "Cannot open file: " << qPrintable(inFile.errorString()) << endl;
        return 1;
    }
    QDir outDir(argv[2]);
    canvas = QImage(400, 300, QImage::Format_RGB32); /* default size */
    color = Qt::black; /* default color */

    /* parse command */
    QTextStream in(&inFile);
    while (!in.atEnd()) {
        QStringList fields = in.readLine().split(' ');
        if (fields.isEmpty()) {
            cerr << "Parse command error: too few arguments." << endl;
            return 1;
        }
        QString cmd = fields.takeFirst();
        if (cmd == "resetCanvas") {
            if (fields.size() != 2) {
                cerr << "Reset canvas error: 2 arguments expected." << endl;
                return 1;
            }
            int width = fields.takeFirst().toInt();
            int height = fields.takeFirst().toInt();
            resetCanvas(width, height);
        }
        else if (cmd == "saveCanvas") {
            if (fields.size() != 1) {
                cerr << "Save canvas error: 1 argument expected." << endl;
                return 1;
            }
            QString name = fields.takeFirst();
            saveCanvas(outDir.filePath(name));
        }
        else if (cmd == "setColor") {
            if (fields.size() != 3) {
                cerr << "Set color error: 3 argument expected." << endl;
                return 1;
            }
            int r = fields.takeFirst().toInt();
            int g = fields.takeFirst().toInt();
            int b = fields.takeFirst().toInt();
            setColor(QColor(r, g, b));
        }
        else if (cmd == "drawLine") {
            if (fields.size() != 6) {
                cerr << "Draw line error: 6 argument expected." << endl;
                return 1;
            }
            int id = fields.takeFirst().toInt();
            int x1 = fields.takeFirst().toInt();
            int y1 = fields.takeFirst().toInt();
            int x2 = fields.takeFirst().toInt();
            int y2 = fields.takeFirst().toInt();
            QString alg = fields.takeFirst();
            drawLine(id, QPoint(x1, y1), QPoint(x2, y2), alg);
        }
        else if (cmd == "drawPolygon") {
            if (fields.size() != 3) {
                cerr << "Draw polygon error: 3 argument expected." << endl;
                return 1;
            }
            int id = fields.takeFirst().toInt();
            int n = fields.takeFirst().toInt();
            QString alg = fields.takeFirst();
            QVector<QPoint> points;
            fields = in.readLine().split(' ');
            if (fields.size() != 2 * n) {
                cerr << "Draw polygon error: " << 2 * n <<
                        " argument expected." << endl;
                return 1;
            }
            while (!fields.isEmpty()) {
                int x = fields.takeFirst().toInt();
                int y = fields.takeFirst().toInt();
                points.append(QPoint(x, y));
            }
            drawPolygon(id, points, alg);
        }
        else if (cmd == "drawEllipse") {
            if (fields.size() != 5) {
                cerr << "Draw ellipse error: 5 argument expected." << endl;
                return 1;
            }
            int id = fields.takeFirst().toInt();
            int x = fields.takeFirst().toInt();
            int y = fields.takeFirst().toInt();
            int rx = fields.takeFirst().toInt();
            int ry = fields.takeFirst().toInt();
            drawEllipse(id, QPoint(x, y), rx, ry);
        }
        else if (cmd == "drawCurve") {
            if (fields.size() != 3) {
                cerr << "Draw curve error: 3 argument expected." << endl;
                return 1;
            }
            int id = fields.takeFirst().toInt();
            int n = fields.takeFirst().toInt();
            QString alg = fields.takeFirst();
            QVector<QPoint> points;
            fields = in.readLine().split(' ');
            if (fields.size() != 2 * n) {
                cerr << "Draw curve error: " << 2 * n <<
                        " argument expected." << endl;
                return 1;
            }
            while (!fields.isEmpty()) {
                int x = fields.takeFirst().toInt();
                int y = fields.takeFirst().toInt();
                points.append(QPoint(x, y));
            }
            drawCurve(id, points, alg);
        }
        else if (cmd == "translate") {
            if (fields.size() != 3) {
                cerr << "Translate error: 3 argument expected." << endl;
                return 1;
            }
            int id = fields.takeFirst().toInt();
            int dx = fields.takeFirst().toInt();
            int dy = fields.takeFirst().toInt();
            translate(id, QPoint(dx, dy));
        }
        else if (cmd == "rotate") {
            if (fields.size() != 4) {
                cerr << "Rotate error: 4 argument expected." << endl;
                return 1;
            }
            int id = fields.takeFirst().toInt();
            int x = fields.takeFirst().toInt();
            int y = fields.takeFirst().toInt();
            int r = fields.takeFirst().toInt();
            rotate(id, QPoint(x, y), r);
        }
        else if (cmd == "scale") {
            if (fields.size() != 4) {
                cerr << "Scale error: 4 argument expected." << endl;
                return 1;
            }
            int id = fields.takeFirst().toInt();
            int x = fields.takeFirst().toInt();
            int y = fields.takeFirst().toInt();
            int s = fields.takeFirst().toInt();
            scale(id, QPoint(x, y), s);
        }
        else if (cmd == "clip") {
            if (fields.size() != 6) {
                cerr << "Clip error: 6 argument expected." << endl;
                return 1;
            }
            int id = fields.takeFirst().toInt();
            int x1 = fields.takeFirst().toInt();
            int y1 = fields.takeFirst().toInt();
            int x2 = fields.takeFirst().toInt();
            int y2 = fields.takeFirst().toInt();
            QString alg = fields.takeFirst();
            clip(id, QPoint(x1, y1), QPoint(x2, y2), alg);
        }
        else {
            cerr << "Undefined command: " << qPrintable(cmd) << endl;
            return 1;
        }
    }

    return 0;
}

void PainterCLI::resetCanvas(int width, int height)
{
    canvas = QImage(width, height, QImage::Format_RGB32);
}

void PainterCLI::saveCanvas(const QString &name)
{
    qDebug() << name;
}

void PainterCLI::setColor(const QColor &color)
{
    this->color = color;
}

void PainterCLI::drawLine(int id, const QPoint &p1, const QPoint &p2,
                          const QString &alg)
{
    qDebug() << id << p1 << p2 << alg;
}

void PainterCLI::drawPolygon(int id, const QVector<QPoint> &points,
                             const QString &alg)
{
    qDebug() << id << points << alg;
}

void PainterCLI::drawEllipse(int id, const QPoint &center, int rx, int ry)
{
    qDebug() << id << center << rx << ry;
}

void PainterCLI::drawCurve(int id, const QVector<QPoint> &points,
                           const QString &alg)
{
    qDebug() << id << points << alg;
}

void PainterCLI::translate(int id, const QPoint &d)
{
    qDebug() << id << d;
}

void PainterCLI::rotate(int id, const QPoint &c, double r)
{
    qDebug() << id << c << r;
}

void PainterCLI::scale(int id, const QPoint &c, double s)
{
    qDebug() << id << c << s;
}

void PainterCLI::clip(int id, const QPoint &topLeft, const QPoint &bottomRight,
                      const QString &alg)
{
    qDebug() << id << topLeft << bottomRight << alg;
}


