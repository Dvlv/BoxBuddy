#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "managepage.h"
#include "newboxpage.h"
#include "overviewpage.h"
#include "src/distrobox.h"
#include <QMainWindow>
#include <vector>

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);

    std::vector<Distrobox::DBox> m_dboxes{};
    std::unordered_map<std::string, std::string> m_distroIcons;

  private:
    OverviewPage *m_overviewpage = nullptr;
    ManagePage *m_managepage = nullptr;
    NewBoxPage *m_newboxpage = nullptr;

    void showOverviewPage();
    void showManagePage(int index);
    void showNewBoxPage();

  private slots:
    void onAddNewButtonClicked();
    void onManageButtonClicked(int index);
    void onBackButtonClicked();
};
#endif // MAINWINDOW_H
