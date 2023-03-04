#ifndef NEWBOXPAGE_H
#define NEWBOXPAGE_H

#include <QScrollArea>
#include <QWidget>
#include <qpushbutton.h>

#include <memory>
#include <vector>

using std::shared_ptr;

class NewBoxPage : public QScrollArea {
    Q_OBJECT

  public:
    NewBoxPage(QWidget *parent = nullptr);

    shared_ptr<QPushButton> m_createButton = nullptr;
    shared_ptr<QPushButton> m_backButton = nullptr;
};
#endif // NEWBOXPAGE_H
