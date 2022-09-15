#include<iostream>
#include<QtGui>
#include<QMainWindow>
#include<QApplication>
#include"main_window.h"


int main(int argc, char** argv)
{

    QApplication app(argc, argv);
    rviz::main_window windows;
    windows.show();

    return app.exec();
}
