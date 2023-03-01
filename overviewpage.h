#ifndef OVERVIEWPAGE_H
#define OVERVIEWPAGE_H

#include <QScrollArea>
#include <QWidget>
#include <qpushbutton.h>

#include <memory>
#include <vector>

using std::shared_ptr;

class OverviewPage : public QScrollArea {
    Q_OBJECT

  public:
    OverviewPage(QWidget *parent = nullptr);

    shared_ptr<QPushButton> m_newButton = nullptr;

    std::vector<shared_ptr<QPushButton>> m_buttons{};
};
#endif // OVERVIEWPAGE_H
