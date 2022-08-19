#include "cmd_button.h"
#include "ui_cmd_button.h"

cmd_button::cmd_button(QWidget *parent,ros::NodeHandle& nh) :
  QWidget(parent),
  ui(new Ui::cmd_button)
{
  nh_ = nh;
  ui->setupUi(this);
  ui->topic_name->setText(QString("/cmd_vel_movebase"));
  ui->line_speed->setMinimum(0.0);
  ui->line_speed->setMaximum(10.0);
  ui->line_speed->setValue(3.5);//设置默认参数
  ui->angle_speed->setMinimum(0.0);
  ui->angle_speed->setMaximum(5.0);
  ui->angle_speed->setValue(1.5);//设置默认参数


//  ui->top_left->setShortcut(QKeySequence(QLatin1String("u")));
//  ui->top->setShortcut(QKeySequence(QLatin1String("i")));
//  ui->top_right->setShortcut(QKeySequence(QLatin1String("o")));
//  ui->left->setShortcut(QKeySequence(QLatin1String("j")));
//  ui->stop->setShortcut(QKeySequence(QLatin1String("k")));
//  ui->right->setShortcut(QKeySequence(QLatin1String("l")));
//  ui->back_left->setShortcut(QKeySequence(QLatin1String("m")));
//  ui->back->setShortcut(QKeySequence(QLatin1String(",")));
//  ui->back_right->setShortcut(QKeySequence(QLatin1String(".")));
  
//  connect(ui->top_left,&QPushButton::clicked,this,[=](){
//    QString Topic_Name = ui->topic_name->text();
//    QString Line_Speed = ui->line_speed->text();
//    qDebug()<<"topic name:"<<Topic_Name<<"Line speed"<<Line_Speed<<"angle speed"<<"times:"<<count;
////    ROS_INFO("%s",msg.data.c_str());
//    chatter_pub.publish(msg);
//    ++count;
//  });

  //绑定速度控制按钮
  connect(ui->top_left,&QPushButton::pressed,this,&cmd_button::slot_cmd_control);
  connect(ui->top,&QPushButton::clicked,this,&cmd_button::slot_cmd_control);
  connect(ui->top_right,&QPushButton::clicked,this,&cmd_button::slot_cmd_control);
  connect(ui->left,&QPushButton::clicked,this,&cmd_button::slot_cmd_control);
  connect(ui->stop,&QPushButton::clicked,this,&cmd_button::slot_cmd_control);
  connect(ui->right,&QPushButton::clicked,this,&cmd_button::slot_cmd_control);
  connect(ui->back_left,&QPushButton::clicked,this,&cmd_button::slot_cmd_control);
  connect(ui->back,&QPushButton::clicked,this,&cmd_button::slot_cmd_control);
  connect(ui->back_right,&QPushButton::clicked,this,&cmd_button::slot_cmd_control);
}
//
void cmd_button::slot_cmd_control()
{
  //topic的名称
  output_topic_ = ui->topic_name->text();
  velocity_publisher_ = nh_.advertise<geometry_msgs::Twist>(output_topic_.toStdString(),1);
  //接收按键信号
  QPushButton* btn=qobject_cast<QPushButton*>(sender());
  char key = btn->text().toStdString()[0];//接收参数
  qDebug()<<QString(key);

  //先判断键值是否在map中存在。
  if(moveBindings.find(key) != moveBindings.end())
  {
    x = moveBindings[key][0];
    th = moveBindings[key][1];
  }
  else if (key == 'k')
  {
    x = 0;
    th = 0;
    control_speed = 0;
    control_turn = 0;
  }
  //增加一个判断，当两次按钮不一样，速度归零
  if(key_static != key)
  {
    x = 0;
    th = 0;
    control_speed = 0;
    control_turn = 0;
  }
  key_static = key;
  //获取当先阈值速度
  target_speed = (ui->line_speed->value()) * x;
  target_turn = (ui->angle_speed->value()) * th;

  //速度限位
  control_speed = target_speed>control_speed ? std::min(target_speed,control_speed + 0.06) : std::max(target_speed,control_speed - 0.06);
  control_turn = target_turn>control_turn ? std::min(target_turn,control_turn + 0.06) : std::max(target_turn,control_turn - 0.06);
  //发布速度
  geometry_msgs::Twist twist;
  twist.linear.x = control_speed;
  twist.linear.y = 0;
  twist.linear.z = 0;

  twist.angular.x = 0;
  twist.angular.y = 0;
  twist.angular.z = control_turn; 

  velocity_publisher_.publish(twist);

  qDebug() <<"contorl_speed"<<QString::number(control_speed);
  qDebug() <<"contorl_turn"<<QString::number(control_turn);
}
/////////////////////////////////////////////////////////////
cmd_button::~cmd_button()
{
  delete ui;
}


















