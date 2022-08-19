#ifndef CMD_BUTTON_H
#define CMD_BUTTON_H

#include <QWidget>
#include <iostream>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <QDebug>

namespace Ui {
class cmd_button;
}

class cmd_button : public QWidget
{
  Q_OBJECT

public:
  explicit cmd_button(QWidget *parent,ros::NodeHandle& nh);
  ~cmd_button();

private:
  Ui::cmd_button *ui;
  ros::Publisher velocity_publisher_;
  ros::NodeHandle nh_;

  std::map<char,std::vector<float>>moveBindings
  {
    //1 正方向 -1 反方向
    {'i',{1,0}},
    {'o',{1,-1}},
    {'j',{0,1}},
    {'l',{0,-1}},
    {'u',{1,1}},
    {',',{-1,0}},
    {'.',{-1,1}},
    {'m',{-1,-1}}
  };
  int x = 0;
  int th = 0;
  double target_speed = 0;
  double target_turn = 0;
  double control_speed = 0;
  double control_turn = 0;
  QString output_topic_;
  char key_static = 'k';

public:
  void slot_cmd_control();
};

#endif // CMD_BUTTON_H
