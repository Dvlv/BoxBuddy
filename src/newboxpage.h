#ifndef NEWBOXPAGE_H
#define NEWBOXPAGE_H

#include "dbbackgroundworker.h"
#include "distrobox.h"
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QScrollArea>
#include <QThread>
#include <QWidget>
#include <qlineedit.h>
#include <qprogressbar.h>
#include <qpushbutton.h>

#include <memory>
#include <qwidget.h>
#include <string>
#include <vector>

using std::shared_ptr;

class NewBoxPage : public QWidget {
    Q_OBJECT
    QThread workerThread;

  public:
    NewBoxPage(
        QWidget *parent = nullptr,
        std::unordered_map<std::string, std::string> *distroIcons = nullptr);
    virtual ~NewBoxPage();

    shared_ptr<QPushButton> m_createButton = nullptr;
    shared_ptr<QPushButton> m_backButton = nullptr;
    shared_ptr<QPushButton> m_homeDirSelectButton = nullptr;

  public slots:
    void onFormSubmit();
    void onBoxCreated(QString result);
    void onSelectHomeDirClicked();

  signals:
    void newBoxCreated();
    void doCreate(QString name, QString distro, QString homeDir);

  private:
    std::vector<std::string> m_images;
    QLineEdit *m_nameEdit = nullptr;
    QLineEdit *m_homeDir = nullptr;
    QComboBox *m_distroSelect = nullptr;
    QLabel *m_outputLabel = nullptr;
    QProgressBar *m_progressBar = nullptr;
    Worker *m_worker = nullptr;

    std::unordered_map<std::string, std::string> *m_distroIcons = nullptr;
};

#endif // NEWBOXPAGE_H
