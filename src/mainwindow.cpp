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
    // TODO xdg_data_dir
    m_distroIcons = {
        {"alma", "data/imgs/alma.png"},
        {"alpine", "data/imgs/alpine.png"},
        {"arch", "data/imgs/arch.png"},
        {"clearlinux", "data/imgs/clearlinux.png"},
        {"centos", "data/imgs/centos.svg"},
        {"debian", "data/imgs/debian.png"},
        {"fedora", "data/imgs/fedora.svg"},
        {"gentoo", "data/imgs/gentoo.png"},
        {"opensuse", "data/imgs/opensuse.png"},
        {"redhat", "data/imgs/redhat.png"},
        {"rocky", "data/imgs/rocky.png"},
        {"slackware", "data/imgs/slackware.png"},
        {"void", "data/imgs/void.png"},
        {"ubuntu", "data/imgs/ubuntu.svg"},
    };

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

    setCentralWidget(m_managepage);
}
