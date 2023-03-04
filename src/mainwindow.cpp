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

    showOverviewPage();
}

void MainWindow::onAddNewButtonClicked() { showNewBoxPage(); }

void MainWindow::onManageButtonClicked(int index) { showManagePage(index); }

void MainWindow::onBackButtonClicked() { showOverviewPage(); }

void MainWindow::showOverviewPage() {
    m_overviewpage = new OverviewPage(this, m_dboxes);

    // new button
    connect(m_overviewpage->m_newButton.get(), &QPushButton::released, this,
            &MainWindow::onAddNewButtonClicked);

    // distro buttons
    connect(m_overviewpage, &OverviewPage::buttonClicked, this,
            &MainWindow::onManageButtonClicked);

    setCentralWidget(m_overviewpage);
}

void MainWindow::showNewBoxPage() {
    m_newboxpage = new NewBoxPage();

    // back
    connect(m_newboxpage->m_backButton.get(), &QPushButton::released, this,
            &MainWindow::onBackButtonClicked);

    setCentralWidget(m_newboxpage);
}

void MainWindow::showManagePage(int index) {
    m_managepage = new ManagePage(this, m_dboxes.at(index));

    // back
    connect(m_managepage->m_backButton.get(), &QPushButton::released, this,
            &MainWindow::onBackButtonClicked);

    setCentralWidget(m_managepage);
}
