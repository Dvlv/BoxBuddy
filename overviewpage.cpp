#include "overviewpage.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include <memory>
#include <qfont.h>
#include <qicon.h>
#include <qlabel.h>
#include <qlayoutitem.h>
#include <qscrollarea.h>
#include <vector>

OverviewPage::OverviewPage(QWidget *parent) : QScrollArea(parent) {
    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(30);

    // Title at top
    QFont font = this->font();
    font.setPointSize(20);

    QHBoxLayout *hbox = new QHBoxLayout();
    QLabel *label = new QLabel("My Boxes");
    label->setFont(font);

    // button for each installed box
    // TODO this is mock data to be fetched from DB
    std::shared_ptr<QPushButton> button1 =
        std::make_shared<QPushButton>("Ubuntu");
    std::shared_ptr<QPushButton> button2 =
        std::make_shared<QPushButton>("Fedora");
    std::shared_ptr<QPushButton> button3 =
        std::make_shared<QPushButton>("Arch");
    std::shared_ptr<QPushButton> button4 =
        std::make_shared<QPushButton>("Debian");

    m_buttons.push_back(button1);
    m_buttons.push_back(button2);
    m_buttons.push_back(button3);
    m_buttons.push_back(button4);

    // for (int i = 0; i < 20; i++) {
    // buttons.push_back(new QPushButton("Ubuntu"));
    //}

    // button for creating new
    QIcon addIcon = QIcon::fromTheme("list-add");
    m_newButton = std::make_shared<QPushButton>(addIcon, "Create New Box");
    m_buttons.push_back(m_newButton);

    // add buttons to grid
    for (int i = 0; i < m_buttons.size(); i++) {
        int row = i / 2;
        int col = i % 2;

        grid->addWidget(m_buttons.at(i).get(), row, col);
    }

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // main view widget
    QWidget *scrollingWidget = new QWidget();
    scrollingWidget->setSizePolicy(QSizePolicy::Expanding,
                                   QSizePolicy::Expanding);

    QVBoxLayout *vbox = new QVBoxLayout();

    hbox->addStretch(1);
    hbox->addWidget(label);
    hbox->addStretch(1);

    vbox->addItem(hbox);
    vbox->addSpacerItem(new QSpacerItem(0, 30));
    vbox->addItem(grid);
    vbox->addStretch(1);

    scrollingWidget->setLayout(vbox);

    this->setWidget(scrollingWidget);
    this->setWidgetResizable(true);
}
