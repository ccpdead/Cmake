#include "settings.h"
#include "ui_settings.h"

settings::settings(QWidget *parent) : QWidget(parent),
                                      ui(new Ui::settings)
{
  ui->setupUi(this);

QSettings main_setting("topic_setting","monitor");
  ui->lineEdit_odom->setText(main_setting.value("topic_odom", "raw_odom").toString());
  ui->lineEdit_power->setText(main_setting.value("topic_power", "power").toString());

  //button_ok
  connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(slot_ok_btn_click()));
  //button_cancel
  connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(slot_cancel_btn_click()));
}

//ok按钮槽函数
void settings::slot_ok_btn_click()
{
  QSettings main_setting("topic_setting","monitor");
  main_setting.setValue("topic_odom",ui->lineEdit_odom->text());
  main_setting.setValue("topic_power",ui->lineEdit_power->text());
  QMessageBox::critical(NULL,"保存成功！","保存成功，部分功能需重启后使用",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
  this->close();

}
//cancel按钮槽函数
void settings::slot_cancel_btn_click(){
  this->close();
}

settings::~settings()
{
  delete ui;
}
