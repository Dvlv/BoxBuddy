#include "distrobox.h"
#include "mainwindow.h"
#include <iostream>

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(400, 600);
    w.show();

    return a.exec();

    /*
    auto boxes = Distrobox::getAllBoxes();

    for (auto box : boxes) {
        std::cout << box.name << " " << box.distro << " " << box.ID
                  << std::endl;
    }

    std::cout << std::endl;

    for (auto app : Distrobox::getLocalApplications("tumbly")) {
        std::cout << app << std::endl;
    }
    */
}
