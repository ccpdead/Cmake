#ifndef QNODE_H
#define QNODE_H

#include <ros/ros.h>
#include <string>
#include <QThread>
#include <QStringListModel>
#include <std_msgs/String.h>

namespace msg{

class qnode : public QThread{

    Q_OBJECT
  public:
    //￥ 构造函数
    qnode(int argc, char** argv);
    virtual ~qnode();

    //$ 初始化
    bool init();
    bool init(const std::string& master_url, const std::string& host_url);

    //循环发送节点
    void run();

    //logging
    enum LogLevel{
      Debug,
      Info,
      Warn,
      Error,
      Fatal
    };

    //返回ros的info给QListsView
    QStringListModel* loggingModel(){return &logging_model;}
    void log(const LogLevel& level,const std::string& msg);
    //返回ros的info给QListsView
    QStringListModel* loggingModel_sub(){return &logging_model_sub;}
    void log_sub(const LogLevel& level,const std::string& msg);


    void Callback(const std_msgs::StringConstPtr& submsg);
    
  //$ 自定义信号
  Q_SIGNALS:
    void loggingUpdated();
    void rosShutdown();
    void loggingUpdated_sub();



  private:
    //$ 存储什么信息？？？
    int init_argc;
    char** init_argv;

    ros::Publisher chatter_publisher;
    QStringListModel logging_model;
    ros::Subscriber chatter_subscriber;
    QStringListModel logging_model_sub;

};


}//namespace
#endif // QNODE_H
