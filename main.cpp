#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << "Application started.";
    MainWindow w;
    w.show();
    qDebug() << "MainWindow shown.";
    return a.exec();
}
