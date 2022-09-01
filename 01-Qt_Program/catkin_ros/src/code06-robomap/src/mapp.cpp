#include "mapp.h"
#include "ui_mapp.h"


namespace robomap{

mapp::mapp(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MAPP)
{
  std::cout<<"map构造函数"<<std::endl;
  ui->setupUi(this);
  int argc = 0;
  char** argv = nullptr;
  ros_init(argc, argv);
  render_planel_ = new rviz::RenderPanel;
  //设定rviz显示的窗口
  ui->verticalLayout->addWidget(render_planel_);
  manager_ = new rviz::VisualizationManager(render_planel_);
  render_planel_->initialize(manager_->getSceneManager(),manager_);
  manager_->initialize();
  manager_->startUpdate();

  QObject::connect(ui->btn_map,SIGNAL(clicked()),this, SLOT(on_btn_map_clicked()));
  QObject::connect(ui->btn_quit,SIGNAL(clicked()),this, SLOT(close()));

}

mapp::~mapp()
{
  std::cout<<"map析构函数"<<std::endl;
  delete ui;
}

void mapp::ros_init(int argc, char** argv){
  std::cout<<"ros init"<<std::endl;
  ros::init(argc,argv,"rosmap",ros::init_options::AnonymousName);
}

void mapp::on_btn_map_clicked()
{
  manager_->removeAllDisplays();
  rviz::Display* map_ = manager_->createDisplay("rviz/Map","adjustable map",true);
  ROS_ASSERT(map_ != NULL);
  //订阅map主题
  map_->subProp("Topic")->setValue("/rtabmap/grid_map");

//  rviz::Display* robot_ = manager_->createDisplay("rviz/RobotModel","adjustable robot",true);
//  ROS_ASSERT(robot_ != NULL);
//  robot_->subProp("Robot Discription")->setValue("robot_discription");

//  rviz::Display* laser_ = manager_->createDisplay("rviz/LaserScan","adjustable scan",true);
//  ROS_ASSERT(laser_ != NULL);
//  laser_->subProp("Topic")->setValue("/scan");
//  laser_->subProp("Size(m)")->setValue("0.1");

  rviz::Display* axes_= manager_->createDisplay("rviz/Axes","adjustable axes",true);
  ROS_ASSERT(axes_ != NULL);
  axes_->subProp("Reference Frame")->setValue("base_footprint");


  rviz::Display* cloud_ = manager_->createDisplay("rviz/PointCloud2", "adjustable cloud", true);
  ROS_ASSERT(cloud_ != NULL);
  cloud_->subProp("Topic")->setValue("/fusion_cloud");
  cloud_->subProp("Size (m)")->setValue("0.1");
  cloud_->subProp("Color Transformer")->setValue("FlatColor");
  cloud_->subProp("Color")->setValue("239;41;41");
}


}//namespace
