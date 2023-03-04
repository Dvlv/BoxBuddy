#ifndef MANAGEPAGE_H
#define MANAGEPAGE_H

#include <QWidget>
#include <qpushbutton.h>

class ManagePage : public QWidget {
    Q_OBJECT

  public:
    ManagePage(QWidget *parent = nullptr);

    std::shared_ptr<QPushButton> m_backButton = nullptr;
};
#endif // MANAGEPAGE_H
