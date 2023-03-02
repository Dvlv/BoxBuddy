#include "newboxpage.h"
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include <qlabel.h>
#include <qlayoutitem.h>

NewBoxPage::NewBoxPage(QWidget *parent) : QScrollArea(parent) {
    // TODO smart pointers and maybe member vars
    // TODO put the form in its own frame separate form the title

    m_createButton = std::make_shared<QPushButton>("Create");

    QIcon addIcon = QIcon::fromTheme("media-skip-backward");
    m_backButton = std::make_shared<QPushButton>(addIcon, "Back");

    QFont font = this->font();
    font.setPointSize(20);
    QLabel *titleLabel = new QLabel("Create A New Box");
    titleLabel->setFont(font);
    titleLabel->setAlignment(Qt::AlignCenter);

    QLineEdit *nameEdit = new QLineEdit();

    QComboBox *distroSelect = new QComboBox();

    QCheckBox *rootCheckBox = new QCheckBox();

    QFormLayout *layout = new QFormLayout();
    layout->setVerticalSpacing(20);
    layout->addWidget(titleLabel);
    layout->addRow("Name", nameEdit);
    layout->addRow("Distro Image", distroSelect);
    layout->addRow("Root", rootCheckBox);
    layout->addItem(new QSpacerItem(0, 30));
    layout->addRow(m_createButton.get());

    QWidget *scrollingWidget = new QWidget();

    scrollingWidget->setLayout(layout);

    this->setWidget(scrollingWidget);
    this->setWidgetResizable(true);
}
