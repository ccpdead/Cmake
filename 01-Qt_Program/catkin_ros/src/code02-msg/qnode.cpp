#include "qnode.h"
#include <ros/ros.h>
#include <ros/network.h>
#include <string>
#include <std_msgs/String.h>
#include <sstream>
namespace msg{

/**
 * @brief 构造函数
 * 
 * @param argc 
 * @param argv 
 */
qnode::qnode(int argc, char** argv):
init_argc(argc),
init_argv(argv)
{}

/**
 * @brief 当窗口关闭后，调用析构函数关闭ROS
 * 
 */
qnode::~qnode(){
    if(ros::isStarted()){
        ros::shutdown();
        ros::waitForShutdown();
    }
    //$ QThread中的函数
    wait();
}

/**
 * @brief 默认构造函数
 * 
 */
bool qnode::init(){
    ros::init(init_argc, init_argv,"msg");//初始化节点
    //￥ 若为检测到master，则错误
    if(!ros::master::check())
    {
        return false;
    }

    ros::start();
    ros::NodeHandle nh;
    chatter_publisher = nh.advertise<std_msgs::String>("chatter",100);
    chatter_subscriber = nh.subscribe("chatter",100,&qnode::Callback,this);
    start();
    return true;
}

/**
 * @brief 有参构造函数
 * 
 * @param master_url 
 * @param host_url 
 * @return true 
 * @return false 
 */
bool qnode::init(const std::string& master_url, const std::string& host_url)
{
    std::map<std::string,std::string>remappings;
    remappings["__master"] = master_url;
    remappings["__hostname"] = host_url;
    ros::init(remappings,"msg");
    //￥ 若没有检测到mastet，则退出程序
    if(! ros::master::check()){
        return false;
    }

    ros::start();
    ros::NodeHandle nh;
    chatter_publisher = nh.advertise<std_msgs::String>("chatter", 100);
    chatter_subscriber = nh.subscribe("chatter",100,&qnode::Callback,this);
    start();
    return true;
}

void qnode::run()
{
    ros::Rate loop_rate(1);//1HZ
    int count = 0;
    while(ros::ok())
    {
        std_msgs::String msg;
        std::stringstream ss;

        ss<<"hello world"<<count;
        msg.data = ss.str();

        //pub msg
        chatter_publisher.publish(msg);
        log(Info,std::string("I sent: ")+msg.data);
        ros::spinOnce();
        loop_rate.sleep();
        ++count;
    }
    std::cout<<"ROS shutdown,proceeding to close gui"<<std::endl;
    //$ 出发rosShutwodn的信号
    Q_EMIT rosShutdown();
}

void qnode::Callback(const std_msgs::StringConstPtr &submsg)
{
  log_sub(Info, std::string("Success sub:")+submsg->data.c_str());
}

/**
 * @brief 这里的msg是发布的std::string("I sent")+msg.data
 * 
 * @param Info 
 * @param std::string("I sent")+msg.data 
 */
void qnode::log( const LogLevel &level, const std::string &msg) {
	logging_model.insertRows(logging_model.rowCount(),1);
	std::stringstream logging_model_msg;//￥ 拼接字符串用的容器
	switch ( level ) {
		case(Debug) : {
				ROS_DEBUG_STREAM(msg);
				logging_model_msg << "[DEBUG] [" << ros::Time::now() << "]: " << msg;
				break;
		}
		case(Info) : {
				ROS_INFO_STREAM(msg);
				logging_model_msg << "[INFO] [" << ros::Time::now() << "]: " << msg;
				break;
		}
		case(Warn) : {
				ROS_WARN_STREAM(msg);
				logging_model_msg << "[INFO] [" << ros::Time::now() << "]: " << msg;
				break;
		}
		case(Error) : {
				ROS_ERROR_STREAM(msg);
				logging_model_msg << "[ERROR] [" << ros::Time::now() << "]: " << msg;
				break;
		}
		case(Fatal) : {
				ROS_FATAL_STREAM(msg);
				logging_model_msg << "[FATAL] [" << ros::Time::now() << "]: " << msg;
				break;
		}
	}
	QVariant new_row(QString(logging_model_msg.str().c_str()));
    //￥ 将信息显示在 QListView
	logging_model.setData(logging_model.index(logging_model.rowCount()-1),new_row);
    //￥ 触发loggingUpdated信号
	Q_EMIT loggingUpdated(); // used to readjust the scrollbar
}

/**
 * @brief 
 * 
 * @param Info 
 * @param std::string("Success sub:")+submsg->data.c_str() 
 */
void qnode::log_sub( const LogLevel &level, const std::string &msg) {
  logging_model_sub.insertRows(logging_model_sub.rowCount(),1);
  std::stringstream logging_model_msg;
  switch ( level ) {
    case(Debug) : {
        ROS_DEBUG_STREAM(msg);
        logging_model_msg << "[DEBUG] [" << ros::Time::now() << "]: " << msg;
        break;
    }
    case(Info) : {
        ROS_INFO_STREAM(msg);
        logging_model_msg << "[INFO] [" << ros::Time::now() << "]: " << msg;
        break;
    }
    case(Warn) : {
        ROS_WARN_STREAM(msg);
        logging_model_msg << "[INFO] [" << ros::Time::now() << "]: " << msg;
        break;
    }
    case(Error) : {
        ROS_ERROR_STREAM(msg);
        logging_model_msg << "[ERROR] [" << ros::Time::now() << "]: " << msg;
        break;
    }
    case(Fatal) : {
        ROS_FATAL_STREAM(msg);
        logging_model_msg << "[FATAL] [" << ros::Time::now() << "]: " << msg;
        break;
    }
  }
  QVariant new_row(QString(logging_model_msg.str().c_str()));
  logging_model_sub.setData(logging_model_sub.index(logging_model_sub.rowCount()-1),new_row);
  Q_EMIT loggingUpdated_sub(); // used to readjust the scrollbar
}


}//namespace
