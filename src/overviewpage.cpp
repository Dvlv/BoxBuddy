#include "overviewpage.h"
#include "src/distrobox.h"
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

OverviewPage::OverviewPage(QWidget *parent, std::vector<Distrobox::DBox> dboxes)
    : QScrollArea(parent), m_dboxes(dboxes) {
    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(30);

    // Title at top
    QFont font = this->font();
    font.setPointSize(20);

    QLabel *label = new QLabel("My Boxes");
    label->setFont(font);

    // button for each installed box

    for (int i = 0; i < m_dboxes.size(); i++) {
        Distrobox::DBox dbox = m_dboxes[i];

        QPushButton *button = new QPushButton(dbox.name.c_str());
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        button->setCursor(Qt::PointingHandCursor);

        connect(button, &QPushButton::released, this,
                [this, i]() { emit buttonClicked(i); });

        m_buttons.push_back(std::shared_ptr<QPushButton>(button));
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

void OverviewPage::onButtonClicked(int index) { emit buttonClicked(index); }
