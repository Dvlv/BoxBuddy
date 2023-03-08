#ifndef OVERVIEWPAGE_H
#define OVERVIEWPAGE_H

#include "distrobox.h"
#include <QScrollArea>
#include <QWidget>
#include <qpushbutton.h>

#include <memory>
#include <qwidget.h>
#include <vector>

using std::shared_ptr;

class OverviewPage : public QWidget {
    Q_OBJECT

  public:
    OverviewPage(QWidget *parent = nullptr,
                 std::vector<Distrobox::DBox> dboxes = {},
                 std::map<std::string, std::string> *distroIcons = nullptr);

    shared_ptr<QPushButton> m_newButton = nullptr;
    std::vector<shared_ptr<QPushButton>> m_buttons{};

    void onButtonClicked(int index);

    std::vector<Distrobox::DBox> m_dboxes{};
    std::map<std::string, std::string> *m_distroIcons = nullptr;

  signals:
    void buttonClicked(int index);
};
#endif // OVERVIEWPAGE_H
