#include "mainwindow.h"
#include "distrobox.h"
#include "managepage.h"
#include "newboxpage.h"
#include "overviewpage.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include <qfont.h>
#include <qicon.h>
#include <qlabel.h>
#include <qlayoutitem.h>
#include <qobjectdefs.h>
#include <qwidget.h>
#include <vector>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    m_dboxes = Distrobox::getAllBoxes();

    this->setWindowTitle("Box Buddy");

    std::string home = std::getenv("HOME");
    std::string xdgDataDir = home + "/.local/share";
    if (std::getenv("XDG_DATA_HOME") != nullptr) {
        xdgDataDir = std::getenv("XDG_DATA_HOME");
    }

    std::string iconPath = xdgDataDir + "/boxbuddy/imgs/boxbuddy.svg";
    this->setWindowIcon(QIcon(QString::fromStdString(iconPath)));

    m_distroIcons = {
        {"alma", xdgDataDir + "/boxbuddy/imgs/alma.png"},
        {"alpine", xdgDataDir + "/boxbuddy/imgs/alpine.png"},
        {"amazon", xdgDataDir + "/boxbuddy/imgs/amazon.png"},
        {"arch", xdgDataDir + "/boxbuddy/imgs/arch.png"},
        {"centos", xdgDataDir + "/boxbuddy/imgs/centos.svg"},
        {"clearlinux", xdgDataDir + "/boxbuddy/imgs/clearlinux.png"},
        {"debian", xdgDataDir + "/boxbuddy/imgs/debian.png"},
        {"fedora", xdgDataDir + "/boxbuddy/imgs/fedora.svg"},
        {"gentoo", xdgDataDir + "/boxbuddy/imgs/gentoo.png"},
        {"kali", xdgDataDir + "/boxbuddy/imgs/kali.png"},
        {"mageia", xdgDataDir + "/boxbuddy/imgs/mageia.png"},
        {"opensuse", xdgDataDir + "/boxbuddy/imgs/opensuse.png"},
        {"oracle", xdgDataDir + "/boxbuddy/imgs/oracle.png"},
        {"redhat", xdgDataDir + "/boxbuddy/imgs/redhat.png"},
        {"rocky", xdgDataDir + "/boxbuddy/imgs/rocky.png"},
        {"slackware", xdgDataDir + "/boxbuddy/imgs/slackware.png"},
        {"ubuntu", xdgDataDir + "/boxbuddy/imgs/ubuntu.svg"},
        {"void", xdgDataDir + "/boxbuddy/imgs/void.png"},
    };

    std::cout << m_distroIcons["alma"] << std::endl;

    showOverviewPage();
}

void MainWindow::onAddNewButtonClicked() { showNewBoxPage(); }

void MainWindow::onManageButtonClicked(int index) { showManagePage(index); }

void MainWindow::onBackButtonClicked() { showOverviewPage(); }

void MainWindow::showOverviewPage() {
    m_overviewpage = new OverviewPage(this, m_dboxes, &m_distroIcons);

    // new button
    connect(m_overviewpage->m_newButton.get(), &QPushButton::released, this,
            &MainWindow::onAddNewButtonClicked);

    // distro buttons
    connect(m_overviewpage, &OverviewPage::buttonClicked, this,
            &MainWindow::onManageButtonClicked);

    setCentralWidget(m_overviewpage);
}

void MainWindow::showNewBoxPage() {
    m_newboxpage = new NewBoxPage(this, &m_distroIcons);

    // back
    connect(m_newboxpage->m_backButton.get(), &QPushButton::released, this,
            &MainWindow::onBackButtonClicked);

    // refetch boxes and load overview page on create
    connect(m_newboxpage, &NewBoxPage::newBoxCreated, this, [this]() {
        m_dboxes = Distrobox::getAllBoxes();
        showOverviewPage();
    });

    setCentralWidget(m_newboxpage);
}

void MainWindow::showManagePage(int index) {
    std::string distro = m_dboxes.at(index).distro;
    std::string distroIcon = "";
    if (m_distroIcons.find(distro) != m_distroIcons.end()) {
        distroIcon = m_distroIcons.at(distro);
    }
    m_managepage = new ManagePage(this, m_dboxes.at(index), distroIcon);

    // back
    connect(m_managepage->m_backButton.get(), &QPushButton::released, this,
            &MainWindow::onBackButtonClicked);

    // refetch boxes and load overview page on delete
    connect(m_managepage, &ManagePage::boxDeleted, this, [this]() {
        m_dboxes = Distrobox::getAllBoxes();
        showOverviewPage();
    });

    setCentralWidget(m_managepage);
}
