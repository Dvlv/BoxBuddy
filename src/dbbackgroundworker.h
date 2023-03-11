#pragma once

#include "distrobox.h"
#include <QObject>
#include <iostream>
#include <string>
#include <vector>

class Worker : public QObject {
    Q_OBJECT

  signals:
    void distroboxCreated(QString result);
    void distroboxDeleted(bool result);
    void commandRun();
    void appsFetched(std::vector<Distrobox::LocalApp> apps);

  public slots:
    void createDistrobox(QString name, QString image) {
        std::string output =
            Distrobox::createNewBox(name.toStdString(), image.toStdString());

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

    void fetchInstalledApps(QString boxName) {
        std::vector<Distrobox::LocalApp> apps =
            Distrobox::getLocalApplications(boxName.toStdString());

        emit appsFetched(apps);
    }
};
