#include "managepage.h"
#include "dbbackgroundworker.h"
#include "distrobox.h"
#include <QGridLayout>
#include <QMessageBox>
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
#include <qwidget.h>
#include <stdexcept>

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

    connect(upgradeButton, &QPushButton::clicked, this, [this]() {
        try {
            Distrobox::upgradeBox(m_dbox.name);
        } catch (std::runtime_error &e) {
            this->showNoTerminalPopup();
        }
    });

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

    connect(termButton, &QPushButton::clicked, this, [this]() {
        try {
            Distrobox::openTerminal(m_dbox.name);
        } catch (const std::runtime_error &e) {
            this->showNoTerminalPopup();
        }
    });

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
    int id = qRegisterMetaType<std::vector<Distrobox::LocalApp>>();
    m_worker = new Worker;
    m_worker->moveToThread(&workerThread);

    connect(&workerThread, &QThread::finished, m_worker, &QObject::deleteLater);
    connect(this, &ManagePage::runBackgroundCmd, m_worker,
            &Worker::runCommandInBox);

    connect(this, &ManagePage::fetchInstalledApps, m_worker,
            &Worker::fetchInstalledApps);

    connect(m_worker, &Worker::appsFetched, this,
            &ManagePage::renderInstalledAppsPopup);

    workerThread.start();
}

void ManagePage::onDeleteButtonClicked() {
    QMessageBox msgBox;

    QFont font = this->font();
    font.setPixelSize(16);
    msgBox.setFont(font);

    msgBox.setWindowTitle("Are you sure?");
    msgBox.setText("Are you sure you want to delete this box?");
    msgBox.setInformativeText("This cannot be undone!");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    auto ans = msgBox.exec();

    if (ans == QMessageBox::Yes) {
        Distrobox::deleteBox(m_dbox.name);
        emit boxDeleted();
    }
}

void ManagePage::onExportAppClicked() {
    // TODO
    m_popupWindow = new QWidget();
    m_popupWindow->setWindowTitle("Export an App");
    m_popupWindow->setWindowModality(Qt::ApplicationModal);
    m_popupWindow->resize(300, 100);

    QVBoxLayout *vbox = new QVBoxLayout();
    m_popupWindowGrid = new QGridLayout();

    QFont font = this->font();
    font.setPixelSize(18);

    m_successLabel = new QLabel("Loading Apps...");
    m_successLabel->setAlignment(Qt::AlignCenter);
    m_successLabel->setFont(font);

    vbox->addWidget(m_successLabel);
    vbox->addLayout(m_popupWindowGrid);
    vbox->addStretch(1);
    m_popupWindow->setLayout(vbox);

    m_popupWindow->show();

    emit fetchInstalledApps(QString::fromStdString(m_dbox.name));
}

void ManagePage::onExportService() {
    // TODO
}

void ManagePage::renderInstalledAppsPopup(
    std::vector<Distrobox::LocalApp> apps) {

    m_apps = apps;

    QFont font = this->font();
    font.setPixelSize(16);

    for (int i = 0; i < m_apps.size(); ++i) {
        Distrobox::LocalApp app = m_apps.at(i);

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

        connect(exportButton, &QPushButton::clicked, this, [this, app]() {
            bool res =
                Distrobox::exportApplication(m_dbox.name, app.desktopFile);
            if (res) {
                std::string success = app.name + " Exported Successfully!";
                m_successLabel->setText(success.c_str());
            }
        });

        connect(runButton, &QPushButton::clicked, this, [this, app]() {
            emit runBackgroundCmd(QString::fromStdString(app.execName),
                                  QString::fromStdString(m_dbox.name));
            m_popupWindow->close();
        });

        m_popupWindowGrid->addWidget(appIconLabel, i, 0);
        m_popupWindowGrid->addWidget(appLabel, i, 1);
        m_popupWindowGrid->addWidget(runButton, i, 2);
        m_popupWindowGrid->addWidget(exportButton, i, 3);
    }

    if (m_apps.size() == 0) {
        m_successLabel->setText("No Apps Found!");
    } else {
        m_successLabel->setText("");
    }
}

void ManagePage::showNoTerminalPopup() {
    QMessageBox msgBox;

    QFont font = this->font();
    font.setPixelSize(16);
    msgBox.setFont(font);

    msgBox.setWindowTitle("No Terminal Found!");
    msgBox.setText("Could not find a compatible terminal.");
    msgBox.setInformativeText("Please install one of:\n- Konsole\n- Gnome "
                              "Terminal\n- Xterm");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

ManagePage::~ManagePage() {
    workerThread.quit();
    workerThread.wait();
}
