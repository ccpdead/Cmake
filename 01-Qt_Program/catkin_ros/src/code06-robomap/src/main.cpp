#include <iostream>
#include <QtGui>
#include <QApplication>
#include "main_window.hpp"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    robomap::MainWindow w(argc, argv);
    w.show();
    

    return app.exec();
}