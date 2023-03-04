#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "managepage.h"
#include "newboxpage.h"
#include "overviewpage.h"
#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);

  private:
    OverviewPage *m_overviewpage = nullptr;
    ManagePage *m_managepage = nullptr;
    NewBoxPage *m_newboxpage = nullptr;

    void showOverviewPage();
    void showManagePage();
    void showNewBoxPage();

  private slots:
    void onAddNewButtonClicked();
    void onManageButtonClicked(std::string distro);
    void onBackButtonClicked();
};
#endif // MAINWINDOW_H
