#include "distrobox.h"
#include "mainwindow.h"
#include <iostream>

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(450, 600);
    w.show();

    return a.exec();
}
