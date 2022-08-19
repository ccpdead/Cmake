#include"cmd_button.h"
#include<QApplication>
#include"ros/ros.h"

int main(int argc, char*argv[])
{
  ros::init(argc,argv,"cmd_putton");
  ros::NodeHandle nh;

  QApplication a(argc,argv);
  cmd_button w(NULL,nh);
  w.show();

  return a.exec();
}
