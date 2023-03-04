#include "managepage.h"
#include "distrobox.h"
#include <QGridLayout>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <qboxlayout.h>
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

    // export service button
    QPushButton *exportService =
        new QPushButton(exportIcon, "Export A Service");

    // upgrade
    QIcon updateIcon = QIcon::fromTheme("system-software-update-symbolic");
    QPushButton *upgradeButton = new QPushButton(updateIcon, "Upgrade");

    connect(upgradeButton, &QPushButton::clicked, this,
            [this]() { Distrobox::upgradeBox(m_dbox.name); });

    // remove
    QIcon removeIcon = QIcon::fromTheme("edit-delete-symbolic");
    QPushButton *removeButton = new QPushButton(removeIcon, "Delete");

    // TODO delete needs to go back to overview and re-fetch
    // do another signal like onManageButtonClicked
    connect(removeButton, &QPushButton::clicked, this,
            [this]() { Distrobox::deleteBox(m_dbox.name); });

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
