#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "managepage.h"
#include "overviewpage.h"
#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);

  private:
    OverviewPage *m_overviewpage = nullptr;
    ManagePage *m_managepage = nullptr;

  private slots:
    void onAddNewButtonClicked();
};
#endif // MAINWINDOW_H
