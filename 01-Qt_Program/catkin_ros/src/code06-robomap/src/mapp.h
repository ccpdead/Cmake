#ifndef MAPP_H
#define MAPP_H

#include <QWidget>
#include <iostream>
#include "ui_mapp.h"
#include <rviz/visualization_manager.h>
#include <rviz/render_panel.h>
#include <rviz/display.h>
#include <rviz/tool_manager.h>

#include <ros/ros.h>
#include <signal.h>


namespace robomap {


class mapp : public QWidget
{
  Q_OBJECT

public:
  explicit mapp(QWidget *parent = nullptr);
  ~mapp();
  
  void ros_init(int argc, char** argv);
  void shutdown(int sig);

  void setAutoRepeat(bool);
private slots:
  void on_btn_map_clicked();

private:
  Ui::MAPP *ui;

  //实例化rviz组件
  rviz::VisualizationManager *manager_;
  rviz::RenderPanel* render_planel_;
  rviz::Display *map_;


};

}//namespace
#endif // MAPP_H
