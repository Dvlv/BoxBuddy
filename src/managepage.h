#ifndef MANAGEPAGE_H
#define MANAGEPAGE_H

#include "dbbackgroundworker.h"
#include "distrobox.h"
#include <QLabel>
#include <QThread>
#include <QWidget>
#include <qgridlayout.h>
#include <qlineedit.h>
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
    void showClonePopup();
    void onCloneClicked();
    void onCloneCancelClicked();
    void onSaveToFileClicked();

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

    // clone popup
    QWidget *m_clonePopup = nullptr;
    QPushButton *m_cloneButton = nullptr;
    QPushButton *m_cloneCancelButton = nullptr;
    QLineEdit *m_cloneName = nullptr;

    // save to file
    QLineEdit *m_saveToFileLoc = nullptr;
    QLabel *m_savePendingLabel = nullptr;
};
#endif // MANAGEPAGE_H
