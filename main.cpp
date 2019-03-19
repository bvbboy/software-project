#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //w.setFixedSize(800,600);
    w.resize(QSize(700,400));

    return a.exec();
}
