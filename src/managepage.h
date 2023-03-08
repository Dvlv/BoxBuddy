#ifndef MANAGEPAGE_H
#define MANAGEPAGE_H

#include "dbbackgroundworker.h"
#include "distrobox.h"
#include <QLabel>
#include <QThread>
#include <QWidget>
#include <qgridlayout.h>
#include <qpushbutton.h>
#include <vector>

class ManagePage : public QWidget {
    Q_OBJECT
    QThread workerThread;

  public:
    ManagePage(QWidget *parent = nullptr, Distrobox::DBox dbox = {},
               std::string distroIcon = "");
    ~ManagePage();

    std::shared_ptr<QPushButton> m_backButton = nullptr;
    Distrobox::DBox m_dbox;
    std::string m_distroIcon;

    void showNoTerminalPopup();

  public slots:
    void onDeleteButtonClicked();
    void onExportAppClicked();
    void onExportService();
    void renderInstalledAppsPopup(std::vector<Distrobox::LocalApp> apps);

  signals:
    void boxDeleted();
    void runBackgroundCmd(QString cmd, QString boxName);
    void fetchInstalledApps(QString boxName);

  private:
    Worker *m_worker = nullptr;
    QWidget *m_popupWindow = nullptr;
    std::vector<Distrobox::LocalApp> m_apps;
    QLabel *m_successLabel = nullptr;
    QGridLayout *m_popupWindowGrid = nullptr;
};
#endif // MANAGEPAGE_H
