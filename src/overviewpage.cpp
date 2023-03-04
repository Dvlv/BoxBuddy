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
#include <qboxlayout.h>
#include <qfont.h>
#include <qframe.h>
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

    // connect buttons to signal which emits the box being opened
    for (auto &btn : m_buttons) {
        std::string dn = btn->text().toStdString();
        connect(btn.get(), &QPushButton::released, this,
                [dn, this]() { onButtonClicked(dn); });
    }

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

    QFrame *gridFrame = new QFrame();
    gridFrame->setFrameShape(QFrame::StyledPanel);
    gridFrame->setFrameShadow(QFrame::Raised);
    gridFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // gridFrame->setStyleSheet("QFrame { border: 1px dashed #acacac; "
    //"background-color: #dbdbdb; padding:40px; }");
    gridFrame->setLayout(grid);

    QHBoxLayout *hbox = new QHBoxLayout();

    hbox->addStretch(1);
    hbox->addWidget(label);
    hbox->addStretch(1);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addItem(hbox);
    vbox->addSpacerItem(new QSpacerItem(0, 30));
    vbox->addWidget(gridFrame);
    vbox->addStretch(1);

    scrollingWidget->setLayout(vbox);

    this->setLayout(vbox);
}

void OverviewPage::onButtonClicked(std::string distro) {
    emit buttonClicked(distro);
}
