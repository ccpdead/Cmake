#include<iostream>
#include<QtGui>
#include<QMainWindow>
#include<mainwindow.h>
#include "ui_mainwindow.h"
#include<QApplication>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  msg::MainWindow w(argc, argv);
  w.show();
  // app.connect(&app,SIGNAL(lastWindowClosed()), &app, SLOT(quit()));

  return app.exec();
}
