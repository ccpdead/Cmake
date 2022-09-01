#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include "ui_main_window.h"

#include "mapp.h"
#include "qnode.hpp"

namespace robomap {


class MainWindow : public QMainWindow {
Q_OBJECT

public:
	MainWindow(int argc, char** argv, QWidget *parent = 0);
	~MainWindow();

	void ReadSettings(); // Load up qt program settings at startup
	void WriteSettings(); // Save qt program settings when closing

	void closeEvent(QCloseEvent *event); // Overloaded function
	void showNoMasterMessage();

//￥ 定义槽函数
public Q_SLOTS:

    void on_button_connect_clicked(bool check );
    void on_checkbox_use_environment_stateChanged(int state);

    void updateLoggingView();

public slots:
	void on_mapping_clicked();

private:

	Ui::main_window* ui = new Ui::main_window();
	QNode qnode;
};

}//namespace


#endif // MAIN_WINDOW_H
