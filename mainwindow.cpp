#include "mainwindow.h"
#include "managepage.h"
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
#include <qwidget.h>
#include <vector>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    m_overviewpage = new OverviewPage();
    // buttons
    connect(m_overviewpage->m_newButton.get(), &QPushButton::released, this,
            &MainWindow::onAddNewButtonClicked);

    setCentralWidget(m_overviewpage);
}

void MainWindow::onAddNewButtonClicked() {
    if (m_managepage == nullptr) {
        m_managepage = new ManagePage();
    }
    setCentralWidget(m_managepage);
}
