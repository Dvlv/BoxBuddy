#include "newboxpage.h"
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include <qboxlayout.h>
#include <qframe.h>
#include <qlabel.h>
#include <qlayoutitem.h>

NewBoxPage::NewBoxPage(QWidget *parent) : QScrollArea(parent) {
    // TODO smart pointers and maybe member vars
    // TODO put the form in its own frame separate form the title

    // ----- form
    m_createButton = std::make_shared<QPushButton>("Create");

    QIcon backIcon = QIcon::fromTheme("go-previous-symbolic");
    m_backButton = std::make_shared<QPushButton>(backIcon, "Back");

    QLineEdit *nameEdit = new QLineEdit();

    QComboBox *distroSelect = new QComboBox();

    QCheckBox *rootCheckBox = new QCheckBox();

    QFormLayout *form = new QFormLayout();
    form->setVerticalSpacing(20);
    form->setHorizontalSpacing(20);
    form->addRow("Name", nameEdit);
    form->addRow("Distro Image", distroSelect);
    form->addRow("Root", rootCheckBox);
    form->addItem(new QSpacerItem(0, 10));
    form->addRow(m_createButton.get());

    QFrame *formFrame = new QFrame();
    formFrame->setFrameShadow(QFrame::Raised);
    formFrame->setFrameShape(QFrame::StyledPanel);
    formFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    formFrame->setLayout(form);

    // ------ title
    QFont font = this->font();
    font.setPointSize(20);
    QLabel *titleLabel = new QLabel("Create A New Box");
    titleLabel->setFont(font);
    titleLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(m_backButton.get());
    hbox->addStretch(1);
    hbox->addWidget(titleLabel);
    hbox->addStretch(1);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addItem(hbox);
    vbox->addWidget(formFrame);
    vbox->addStretch(1);

    this->setLayout(vbox);
    this->setWidgetResizable(true);
}
