#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include<qnode.h>

namespace msg {


class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  // explicit MainWindow(QWidget *parent = nullptr);
  MainWindow(int argc, char** argv, QWidget* parent = 0);
  ~MainWindow();

  void ReadSettings();
  void WriteSettings();

  void closeEvent(QCloseEvent* event);
  void showNoMasterMessage();

//$ 自定义信号槽
public Q_SLOTS:

void on_actionAbout_triggered();
void on_button_connect_clicked(bool check);
void on_checkbox_use_environment_stateChanged(int state);

void updateLoggingView();
void updateLoggingView_sub();
void pub_cmd();



private:
  Ui::MainWindow *ui = new Ui::MainWindow();
  //$ 实例化ros master配置
  qnode QNODE;
};

}//namespace

#endif // MAINWINDOW_H
