#include <QtGui>
#include <QWidget>
#include <QMessageBox>
#include <iostream>

#include "main_window.hpp"
#include "ui_main_window.h"
#include "ui_mapp.h"
#include "mapp.h"
#include "qnode.hpp"
using namespace Qt;


namespace robomap{

MainWindow::MainWindow(int argc, char** argv, QWidget *parent)
	: QMainWindow(parent)
	, qnode(argc,argv)
{
	std::cout<<"mainwindow构造函数"<<std::endl;
	ui->setupUi(this); // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.

  	ReadSettings();
  	QObject::connect(&qnode, SIGNAL(rosShutdown()), this, SLOT(close()));

	ui->view_logging->setModel(qnode.loggingModel());
    QObject::connect(&qnode, SIGNAL(loggingUpdated()), this, SLOT(updateLoggingView()));

	//￥ Auto start
    if ( ui->checkbox_remember_settings->isChecked() ) {
        on_button_connect_clicked(true);
    }

	QObject::connect(ui->quit_button,SIGNAL(clicked()), this, SLOT(close()));

	connect(ui->btn_mapping,SIGNAL(clicked()),this,SLOT(on_mapping_clicked()));

}

MainWindow::~MainWindow(){
	std::cout<<"mainwindow析构函数"<<std::endl;
	delete ui;
}

void MainWindow::showNoMasterMessage(){
	QMessageBox msgBox;
	msgBox.setText("Couldn't find the ros master.");
	msgBox.exec();
    close();
}

void MainWindow::on_button_connect_clicked(bool check){
	if ( ui->checkbox_use_environment->isChecked() ) {
		if ( !qnode.init() ) {
			showNoMasterMessage();
		} else {
			ui->button_connect->setEnabled(false);
		}
	} else {
		if ( ! qnode.init(ui->line_edit_master->text().toStdString(),
				   ui->line_edit_host->text().toStdString()) ) {
			showNoMasterMessage();
		} else {
			ui->button_connect->setEnabled(false);
			ui->line_edit_master->setReadOnly(true);
			ui->line_edit_host->setReadOnly(true);
			ui->line_edit_topic->setReadOnly(true);
		}
	}
}

void MainWindow::on_checkbox_use_environment_stateChanged(int state){
	bool enabled;
	if ( state == 0 ) {
		enabled = true;
	} else {
		enabled = false;
	}
	ui->line_edit_master->setEnabled(enabled);
	ui->line_edit_host->setEnabled(enabled);
}

void MainWindow::updateLoggingView(){
  ui->view_logging->scrollToBottom();
}

void MainWindow::ReadSettings(){
  QSettings settings("Qt-Ros Package", "robomap");
  restoreGeometry(settings.value("geometry").toByteArray());
  restoreState(settings.value("windowState").toByteArray());
  QString master_url = settings.value("master_url",QString("http://192.168.1.2:11311/")).toString();
  QString host_url = settings.value("host_url", QString("192.168.1.3")).toString();
  //QString topic_name = settings.value("topic_name", QString("/chatter")).toString();
  ui->line_edit_master->setText(master_url);
  ui->line_edit_host->setText(host_url);
  //ui.line_edit_topic->setText(topic_name);
  bool remember = settings.value("remember_settings", false).toBool();
  ui->checkbox_remember_settings->setChecked(remember);
  bool checked = settings.value("use_environment_variables", false).toBool();
  ui->checkbox_use_environment->setChecked(checked);
  if ( checked ) {
    ui->line_edit_master->setEnabled(false);
    ui->line_edit_host->setEnabled(false);
    //ui.line_edit_topic->setEnabled(false);
  }
}

void MainWindow::WriteSettings(){
  QSettings settings("Qt-Ros Package", "robomap");
  settings.setValue("master_url",ui->line_edit_master->text());
  settings.setValue("host_url",ui->line_edit_host->text());
  //settings.setValue("topic_name",ui.line_edit_topic->text());
  settings.setValue("use_environment_variables",QVariant(ui->checkbox_use_environment->isChecked()));
  settings.setValue("geometry", saveGeometry());
  settings.setValue("windowState", saveState());
  settings.setValue("remember_settings",QVariant(ui->checkbox_remember_settings->isChecked()));
}

void MainWindow::closeEvent(QCloseEvent* event){
	WriteSettings();
	QMainWindow::closeEvent(event);
}

void MainWindow::on_mapping_clicked()
{
  mapp* MAP = new mapp;
	MAP->show();
	// MAP->exec();
}
}//namespace
