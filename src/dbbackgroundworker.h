#pragma once

#include "distrobox.h"
#include <QObject>
#include <iostream>
#include <string>

class Worker : public QObject {
    Q_OBJECT

  signals:
    void distroboxCreated(QString result);
    void distroboxDeleted(bool result);
    void commandRun();

  public slots:
    void createDistrobox(QString name, QString image, bool root) {
        std::string output = Distrobox::createNewBox(name.toStdString(),
                                                     image.toStdString(), root);

        Distrobox::initNewBox(name.toStdString());

        emit distroboxCreated(QString::fromStdString(output));
    }

    void deleteDistrobox(const std::string &name) {
        bool output = Distrobox::deleteBox(name);
        emit distroboxDeleted(output);
    }

    void runCommandInBox(QString cmd, QString boxName) {
        std::string output =
            Distrobox::runCmdInBox(cmd.toStdString(), boxName.toStdString());
        emit commandRun();
    }
};
