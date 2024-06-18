#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

    // assuming that the positions of all cameras and the shape
    // is such that 0 <= x, y, z, <= 100

}
