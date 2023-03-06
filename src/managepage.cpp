#include "managepage.h"
#include "distrobox.h"
#include <QGridLayout>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <qboxlayout.h>
#include <qfont.h>
#include <qgridlayout.h>
#include <qicon.h>
#include <qlabel.h>
#include <qpixmap.h>

ManagePage::ManagePage(QWidget *parent, Distrobox::DBox dbox,
                       std::string distroIcon)
    : QWidget(parent), m_dbox(dbox), m_distroIcon(distroIcon) {

    QVBoxLayout *vbox = new QVBoxLayout();
    QGridLayout *grid = new QGridLayout();
    QHBoxLayout *hbox = new QHBoxLayout();
    grid->setSpacing(25);

    // TODO distro name
    QLabel *title = new QLabel(m_dbox.name.c_str());
    title->setAlignment(Qt::AlignCenter);

    // TODO distro logo
    QPixmap *logo = new QPixmap(m_distroIcon.c_str());
    QLabel *logoLabel = new QLabel();
    logoLabel->setPixmap(*logo);
    logoLabel->setScaledContents(true);
    logoLabel->setFixedSize(50, 50);

    // generate entry
    QIcon geIcon = QIcon::fromTheme("document-new-symbolic");
    QPushButton *generateEntry = new QPushButton(geIcon, "Add Box to Menu");

    // TODO make this actual method so I can show success popup
    connect(generateEntry, &QPushButton::clicked, this,
            [this]() { Distrobox::addToMenu(m_dbox.name); });

    // export app button
    QIcon exportIcon = QIcon::fromTheme("media-eject-symbolic");
    QPushButton *exportApp = new QPushButton(exportIcon, "Export An App");

    connect(exportApp, &QPushButton::clicked, this,
            &ManagePage::onExportAppClicked);

    // export service button
    QPushButton *exportService =
        new QPushButton(exportIcon, "Export A Service");

    connect(exportService, &QPushButton::clicked, this,
            &ManagePage::onExportService);

    // upgrade
    QIcon updateIcon = QIcon::fromTheme("system-software-update-symbolic");
    QPushButton *upgradeButton = new QPushButton(updateIcon, "Upgrade");

    connect(upgradeButton, &QPushButton::clicked, this,
            [this]() { Distrobox::upgradeBox(m_dbox.name); });

    // remove
    QIcon removeIcon = QIcon::fromTheme("edit-delete-symbolic");
    QPushButton *removeButton = new QPushButton(removeIcon, "Delete");

    connect(removeButton, &QPushButton::clicked, this,
            &ManagePage::onDeleteButtonClicked);

    // launch term
    QIcon termIcon = QIcon::fromTheme("utilities-terminal-symbolic");
    QPushButton *termButton = new QPushButton(termIcon, "Open Terminal");

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
    grid->addWidget(generateEntry, 0, 1);
    grid->addWidget(exportApp, 1, 0);
    grid->addWidget(exportService, 1, 1);
    grid->addWidget(upgradeButton, 2, 0);
    grid->addWidget(removeButton, 2, 1);

    vbox->addLayout(hbox);
    vbox->addSpacerItem(new QSpacerItem(0, 30));
    vbox->addLayout(grid);
    vbox->addStretch(1);

    setLayout(vbox);
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

    std::vector<std::string> apps =
        Distrobox::getLocalApplications(m_dbox.name);

    font.setPixelSize(16);

    for (int i = 0; i < apps.size(); ++i) {

        std::string app = apps.at(i);
        app = app.substr(0, app.length() - 1);
        QIcon appIcon = QIcon::fromTheme(app.c_str()); // strip newline
                                                       //
        appIcon = appIcon.isNull()
                      ? QIcon::fromTheme("application-x-executable")
                      : appIcon;

        QLabel *appLabel = new QLabel(app.c_str());
        appLabel->setFont(font);

        QLabel *appIconLabel = new QLabel();
        appIconLabel->setPixmap(appIcon.pixmap(40, 40));

        QPushButton *runButton = new QPushButton("Run");
        runButton->setFont(font);

        QPushButton *exportButton = new QPushButton("Add to Menu");
        exportButton->setFont(font);

        connect(exportButton, &QPushButton::clicked, this,
                [this, successLabel, app]() {
                    bool res = Distrobox::exportApplication(m_dbox.name, app);
                    if (res) {
                        std::string success = app + " Exported Successfully!";
                        successLabel->setText(success.c_str());
                    }
                });

        // TODO need to parse the desktop file for its command
        connect(runButton, &QPushButton::clicked, this,
                [this, popupWindow, app]() {
                    Distrobox::runCmdInBox(app, m_dbox.name);
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
