#include "managepage.h"
#include "dbbackgroundworker.h"
#include "distrobox.h"
#include <QGridLayout>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <cstdlib>
#include <qboxlayout.h>
#include <qfont.h>
#include <qgridlayout.h>
#include <qicon.h>
#include <qlabel.h>
#include <qpixmap.h>

ManagePage::ManagePage(QWidget *parent, Distrobox::DBox dbox,
                       std::string distroIcon)
    : QWidget(parent), m_dbox(dbox), m_distroIcon(distroIcon) {

    QFont font = this->font();
    font.setPixelSize(16);
    this->setFont(font);

    QFont labelFont = this->font();
    labelFont.setPixelSize(20);

    QVBoxLayout *vbox = new QVBoxLayout();
    QGridLayout *grid = new QGridLayout();
    QHBoxLayout *hbox = new QHBoxLayout();
    grid->setSpacing(25);

    QFrame *buttonFrame = new QFrame();
    buttonFrame->setFrameShadow(QFrame::Raised);
    buttonFrame->setFrameShape(QFrame::StyledPanel);
    buttonFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    buttonFrame->setStyleSheet("QFrame {padding: 15px;}");
    buttonFrame->setLayout(grid);

    QLabel *title = new QLabel(m_dbox.name.c_str());
    title->setAlignment(Qt::AlignCenter);
    title->setFont(labelFont);

    QPixmap *logo = new QPixmap(m_distroIcon.c_str());
    QLabel *logoLabel = new QLabel();
    logoLabel->setPixmap(*logo);
    logoLabel->setScaledContents(true);
    logoLabel->setFixedSize(50, 50);

    // export app button
    QIcon exportIcon = QIcon::fromTheme("application-x-executable-symbolic");
    QPushButton *installedAppsButton =
        new QPushButton(exportIcon, "Installed Apps");
    installedAppsButton->setFont(font);

    connect(installedAppsButton, &QPushButton::clicked, this,
            &ManagePage::onExportAppClicked);

    // upgrade
    QIcon updateIcon = QIcon::fromTheme("system-software-update-symbolic");
    QPushButton *upgradeButton = new QPushButton(updateIcon, "Upgrade");
    upgradeButton->setFont(font);

    connect(upgradeButton, &QPushButton::clicked, this,
            [this]() { Distrobox::upgradeBox(m_dbox.name); });

    // remove
    QIcon removeIcon = QIcon::fromTheme("edit-delete-symbolic");
    QPushButton *removeButton = new QPushButton(removeIcon, "Delete");
    removeButton->setFont(font);

    connect(removeButton, &QPushButton::clicked, this,
            &ManagePage::onDeleteButtonClicked);

    // launch term
    QIcon termIcon = QIcon::fromTheme("utilities-terminal-symbolic");
    QPushButton *termButton = new QPushButton(termIcon, "Open Terminal");
    termButton->setFont(font);

    connect(termButton, &QPushButton::clicked, this,
            [this]() { Distrobox::openTerminal(m_dbox.name); });

    // back
    QIcon backIcon = QIcon::fromTheme("go-previous-symbolic");
    m_backButton = std::make_shared<QPushButton>(backIcon, "Back", this);
    m_backButton->setGeometry(10, 10, 80, 40);
    m_backButton->show();

    // hbox->addWidget(m_backButton.get());
    hbox->addStretch(1);
    hbox->addWidget(logoLabel);
    hbox->addWidget(title);
    hbox->addStretch(1);

    grid->addWidget(termButton, 0, 0);
    grid->addWidget(installedAppsButton, 0, 1);
    grid->addWidget(upgradeButton, 1, 0);
    grid->addWidget(removeButton, 1, 1);

    vbox->addLayout(hbox);
    vbox->addSpacerItem(new QSpacerItem(0, 30));
    vbox->addWidget(buttonFrame);
    vbox->addStretch(1);

    setLayout(vbox);

    // async worker
    m_worker = new Worker;
    m_worker->moveToThread(&workerThread);

    connect(&workerThread, &QThread::finished, m_worker, &QObject::deleteLater);
    connect(this, &ManagePage::runBackgroundCmd, m_worker,
            &Worker::runCommandInBox);
    workerThread.start();
}

void ManagePage::onDeleteButtonClicked() {
    Distrobox::deleteBox(m_dbox.name);
    emit boxDeleted();
}

void ManagePage::onExportAppClicked() {
    // TODO
    QWidget *popupWindow = new QWidget();
    popupWindow->setWindowTitle("Export an App");
    popupWindow->setWindowModality(Qt::ApplicationModal);

    QVBoxLayout *vbox = new QVBoxLayout();
    QGridLayout *grid = new QGridLayout();

    QFont font = this->font();
    font.setPixelSize(18);

    QLabel *successLabel = new QLabel(" ");
    successLabel->setAlignment(Qt::AlignCenter);
    successLabel->setFont(font);

    std::vector<Distrobox::LocalApp> apps =
        Distrobox::getLocalApplications(m_dbox.name);

    font.setPixelSize(16);

    for (int i = 0; i < apps.size(); ++i) {
        Distrobox::LocalApp app = apps.at(i);

        QIcon appIcon = QIcon::fromTheme(app.icon.c_str());

        appIcon = appIcon.isNull()
                      ? QIcon::fromTheme("application-x-executable")
                      : appIcon;

        QLabel *appLabel = new QLabel(app.name.c_str());
        appLabel->setFont(font);

        QLabel *appIconLabel = new QLabel();
        appIconLabel->setPixmap(appIcon.pixmap(40, 40));

        QPushButton *runButton = new QPushButton("Run");
        runButton->setFont(font);

        QPushButton *exportButton = new QPushButton("Add to Menu");
        exportButton->setFont(font);

        connect(exportButton, &QPushButton::clicked, this,
                [this, successLabel, app]() {
                    bool res =
                        Distrobox::exportApplication(m_dbox.name, app.name);
                    if (res) {
                        std::string success =
                            app.name + " Exported Successfully!";
                        successLabel->setText(success.c_str());
                    }
                });

        connect(runButton, &QPushButton::clicked, this,
                [this, popupWindow, app]() {
                    emit runBackgroundCmd(QString::fromStdString(app.execName),
                                          QString::fromStdString(m_dbox.name));
                    popupWindow->close();
                });

        grid->addWidget(appIconLabel, i, 0);
        grid->addWidget(appLabel, i, 1);
        grid->addWidget(runButton, i, 2);
        grid->addWidget(exportButton, i, 3);
    }

    vbox->addWidget(successLabel);
    vbox->addLayout(grid);
    popupWindow->setLayout(vbox);

    popupWindow->show();
}

void ManagePage::onExportService() {
    // TODO
}

ManagePage::~ManagePage() {
    workerThread.quit();
    workerThread.wait();
}
