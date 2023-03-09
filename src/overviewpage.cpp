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

OverviewPage::OverviewPage(
    QWidget *parent, std::vector<Distrobox::DBox> dboxes,
    std::unordered_map<std::string, std::string> *distroIcons)
    : QWidget(parent), m_dboxes(dboxes), m_distroIcons(distroIcons) {
    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(30);

    QFont font = this->font();
    font.setPixelSize(16);
    this->setFont(font);

    // Title at top
    QLabel *label = new QLabel("My Boxes");
    QFont labelFont = this->font();
    labelFont.setPixelSize(20);
    label->setFont(labelFont);

    // button for each installed box
    for (int i = 0; i < m_dboxes.size(); i++) {
        Distrobox::DBox dbox = m_dboxes[i];

        QPushButton *button;

        if (m_distroIcons->find(dbox.distro) != m_distroIcons->end()) {
            QPixmap pixmap(
                QString::fromStdString(m_distroIcons->at(dbox.distro)));
            button = new QPushButton(pixmap, dbox.name.c_str());
            button->setIconSize(QSize(40, 40));
        } else {
            button = new QPushButton(dbox.name.c_str());
        }

        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        font.setPixelSize(16);
        button->setFont(font);

        connect(button, &QPushButton::released, this,
                [this, i]() { emit buttonClicked(i); });

        m_buttons.push_back(std::shared_ptr<QPushButton>(button));
    }

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

    this->setLayout(vbox);
}

void OverviewPage::onButtonClicked(int index) { emit buttonClicked(index); }
