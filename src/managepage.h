#ifndef MANAGEPAGE_H
#define MANAGEPAGE_H

#include "dbbackgroundworker.h"
#include "distrobox.h"
#include <QThread>
#include <QWidget>
#include <qpushbutton.h>

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

  public slots:
    void onDeleteButtonClicked();
    void onExportAppClicked();
    void onExportService();

  signals:
    void boxDeleted();
    void runBackgroundCmd(QString cmd, QString boxName);

  private:
    Worker *m_worker = nullptr;
};
#endif // MANAGEPAGE_H
