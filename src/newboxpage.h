#ifndef NEWBOXPAGE_H
#define NEWBOXPAGE_H

#include "distrobox.h"
#include <QCheckBox>
#include <QComboBox>
#include <QScrollArea>
#include <QWidget>
#include <qlineedit.h>
#include <qpushbutton.h>

#include <memory>
#include <string>
#include <vector>

using std::shared_ptr;

class NewBoxPage : public QScrollArea {
    Q_OBJECT

  public:
    NewBoxPage(QWidget *parent = nullptr,
               std::map<std::string, std::string> *distroIcons = nullptr);

    shared_ptr<QPushButton> m_createButton = nullptr;
    shared_ptr<QPushButton> m_backButton = nullptr;

  public slots:
    void onFormSubmit();

  private:
    std::vector<std::string> m_images;
    QLineEdit *m_nameEdit = nullptr;
    QComboBox *m_distroSelect = nullptr;
    QCheckBox *m_rootCheckBox = nullptr;
    std::map<std::string, std::string> *m_distroIcons = nullptr;
};

#endif // NEWBOXPAGE_H
