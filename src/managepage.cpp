#include "managepage.h"
#include <QGridLayout>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <qboxlayout.h>
#include <qicon.h>
#include <qlabel.h>
#include <qpixmap.h>

ManagePage::ManagePage(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *vbox = new QVBoxLayout();
    QGridLayout *grid = new QGridLayout();
    QHBoxLayout *hbox = new QHBoxLayout();
    grid->setSpacing(25);

    // TODO distro name
    QLabel *title = new QLabel("Ubuntu 22.04");
    title->setAlignment(Qt::AlignCenter);

    // TODO distro logo
    QPixmap *logo =
        new QPixmap("/usr/share/icons/hicolor/48x48/apps/qt4-logo.png");
    QLabel *logoLabel = new QLabel();
    logoLabel->setPixmap(*logo);

    // generate entry
    QIcon geIcon = QIcon::fromTheme("document-new-symbolic");
    QPushButton *generateEntry = new QPushButton(geIcon, "Add Box to Menu");

    // export app button
    QIcon exportIcon = QIcon::fromTheme("media-eject-symbolic");
    QPushButton *exportApp = new QPushButton(exportIcon, "Export An App");

    // export service button
    QPushButton *exportService =
        new QPushButton(exportIcon, "Export A Service");

    // upgrade
    QIcon updateIcon = QIcon::fromTheme("system-software-update-symbolic");
    QPushButton *upgradeButton = new QPushButton(updateIcon, "Upgrade");

    // remove
    QIcon removeIcon = QIcon::fromTheme("edit-delete-symbolic");
    QPushButton *removeButton = new QPushButton(removeIcon, "Delete");

    // launch term
    QIcon termIcon = QIcon::fromTheme("utilities-terminal-symbolic");
    QPushButton *termButton = new QPushButton(termIcon, "Open Terminal");

    // back
    QIcon backIcon = QIcon::fromTheme("go-previous-symbolic");
    m_backButton = std::make_shared<QPushButton>(backIcon, "Back");

    hbox->addWidget(m_backButton.get());
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
