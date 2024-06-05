#include "mainwindow.h"
#include "spationode.h"
#include "road.h"
#include <QApplication>
#include <QPushButton>
#include <QPalette>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

//    w.autoFillBackground();
//    QPalette palette;
//    palette.setColor(QPalette::Background,QColor(192,253,123));
//    w.setPalette(palette);

    w.show();
    return a.exec();
}
