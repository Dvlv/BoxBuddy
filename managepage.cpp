#include "managepage.h"
#include <QGridLayout>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

ManagePage::ManagePage(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *vbox = new QVBoxLayout();
    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(10);

    QPushButton *btn1 = new QPushButton("Distro 1");
    QPushButton *btn2 = new QPushButton("Distro 2");

    grid->addWidget(btn1, 0, 0);
    grid->addWidget(btn2, 0, 1);

    vbox->addLayout(grid);
    setLayout(vbox);
}
