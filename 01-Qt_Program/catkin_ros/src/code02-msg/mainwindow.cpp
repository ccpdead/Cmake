#include "mainwindow.h"
#include "QMessageBox"
#include "QtGui"
#include "ui_mainwindow.h"
#include "qnode.h"

namespace msg{

MainWindow::MainWindow(int argc, char**argv,QWidget *parent) :
  QMainWindow(parent),
  QNODE(argc,argv)
{
  ui->setupUi(this);
  //qApp is a global variable for the application
  QObject::connect(ui->actionAbout_Qt,SIGNAL(triggered(bool)),qApp,SLOT(aboutQt()));

  //$ 读取配置
  ReadSettings();
  //$若ros关闭，则调用close槽函数关闭窗口
  QObject::connect(&QNODE,SIGNAL(rosShutdown()),this,SLOT(close()));

  //$链接信号与槽函数
  ui->view_logging->setModel(QNODE.loggingModel());//￥ 在这里将ros的info信息传递给Qlistview
  QObject::connect(&QNODE,SIGNAL(loggingUpdated()),this, SLOT(updateLoggingView()));

  ui->view_logging_sub->setModel(QNODE.loggingModel_sub());//￥ 在这里将ros的info信息传递给Qlistview
  QObject::connect(&QNODE, SIGNAL(loggingUpdated_sub()),this, SLOT(updateLoggingView_sub()));
  
  //$ 当检测到按键被按下后，调用相应的处理函数
  if(ui->checkbox_remember->isChecked())
  {
    on_button_connect_clicked(true);
  }

  //当点击quit时，关闭窗口
  // connect(ui->button_quit,&QPushButton::clicked,this,&QWidget::close);
  connect(ui->button_quit,&QPushButton::clicked,this,&MainWindow::close);
  
  //$ 当按下发送按钮，触发 pub_cmd信号
  QObject::connect(ui->sent_cmd,SIGNAL(clicked()),this, SLOT(pub_cmd()));
}

MainWindow::~MainWindow() {
  std::cout<<"析构函数"<<std::endl;
  delete ui;
}

/**
 * @brief 若没有检测到master，则弹出警告栏 报错
 * 
 */
void MainWindow::showNoMasterMessage(){
  QMessageBox msgBox;
  msgBox.setText("cloun't find the ros master");
  msgBox.exec();
  close();
}


//$ 链接按钮槽函数
void MainWindow::on_button_connect_clicked(bool check){
  if(ui->checkbox_use_environment->isChecked()){
    if(!QNODE.init()){
      showNoMasterMessage();
    }else{
      //$ 若检测到master没有没初始化，则connect按钮不可按下
      ui->button_connect->setEnabled(false);
    }
  }else{
    if(!QNODE.init(ui->line_edit_master->text().toStdString(),
    ui->line_edit_host->text().toStdString())){
      showNoMasterMessage();//@ 显示错误信息
    }else{
      //￥ init初始化成功后，button_connect按钮变成不可点击状态
      ui->button_connect->setEnabled(false);
      ui->line_edit_master->setReadOnly(true);
      ui->line_edit_host->setReadOnly(true);
      ui->line_edit_topic->setReadOnly(true);
    }
  }
}

//复选框使用环境状态已更改
void MainWindow::on_checkbox_use_environment_stateChanged(int state){
  bool enabled;
  if(state == 0){
    enabled = true;
  }else{
    enabled = false;
  }
  ui->line_edit_master->setEnabled(enabled);
  ui->line_edit_host->setEnabled(enabled);
}

//发送成功后，在信息框显示状态
void MainWindow::updateLoggingView(){

  //$ scroolToBottom添加信息到底部
  ui->view_logging->scrollToBottom();
}

//接收信息成功后，在信息框显示状态
void MainWindow::updateLoggingView_sub(){

  //$ scroolToBottom添加信息到底部
  ui->view_logging_sub->scrollToBottom();
}

//点击菜单栏
void MainWindow::on_actionAbout_triggered(){
  QMessageBox::about(this,tr("About...."),tr("ros master"));
}

void MainWindow::ReadSettings(){
  QSettings settings("Qt-Ros Package","msg");
  // restoreGeometry(settings.value("geometry").toByteArray());
  // restoreState(settings.value("windowState").toBitArray());
  QString master_url = settings.value("master_url", QString("http://192.168.1.28:113111/")).toString();
  QString host_url = settings.value("host_url",QString("192.168.1.28")).toString();

  ui->line_edit_master->setText(master_url);
  ui->line_edit_host->setText(host_url);

  bool remember = settings.value("remember_settings", false).toBool();
  ui->checkbox_remember->setChecked(remember);
  
  bool checked = settings.value("use_environment_variables", false).toBool();
  if(checked){
    ui->line_edit_master->setEnabled(false);
    ui->line_edit_host->setEnabled(false);
  }
}


void MainWindow::WriteSettings(){
  QSettings settings("Qt-Ros Package","msg");
  settings.setValue("master_url", ui->line_edit_master->text());
  settings.setValue("host_url", ui->line_edit_host->text());
  settings.setValue("use_environment_variables", QVariant(ui->checkbox_use_environment->isChecked()));
  // settings.setValue("geometry",saveGeometry());
  // settings.setValue("windowState", saveState());
  settings.setValue("remember_settings",QVariant(ui->checkbox_remember->isChecked()));
}

void MainWindow::closeEvent(QCloseEvent* event){
  WriteSettings();
  QMainWindow::closeEvent(event);

}

void MainWindow::pub_cmd()
{
  QNODE.sent_cmd();
}


}//namespace
