#include <ros/ros.h>
#include <ros/network.h>
#include <string>
#include <std_msgs/String.h>
#include <sstream>
#include <qnode.h>
#include <QDebug>

namespace monitor
{
    QNode::QNode(int argc, char **argv) : init_argc(argc),
                                          init_argv(argv)
    {
        //读取topic的设置
        QSettings topic_setting("topic_setting", "monitor");
        odom_topic = topic_setting.value("topic_odom", "raw_odom").toString();
        power_topic = topic_setting.value("topic_power,power").toString();
        pose_topic = topic_setting.value("topic_amcl", "amcl_pose").toString();
        power_min = topic_setting.value("power_min", "10").toString();
        power_max = topic_setting.value("power_max", "12").toString();
    }

    QNode::~QNode()
    {
        if (ros::isStarted())
        {
            ros::shutdown();
            ros::waitForShutdown();
        }
        wait();
    }

    /**
     * @brief 无参初始化函数
     *
     * @return true
     * @return false
     */
    bool QNode::init()
    {
        ros::init(init_argc, init_argv, "monitor");
        if (!ros::master::check())
        {
            return false;
        }

        ros::start();
        ros::NodeHandle n;

        cmdVel_sub = n.subscribe<nav_msgs::Odometry>(odom_topic.toStdString(), 200, &QNode::speedCallback, this);
        power_sub = n.subscribe(power_topic.toStdString(), 1000, &QNode::powerCallback, this);
        //机器人位置话题
        pos_sub = n.subscribe(pose_topic.toStdString(), 1000, &QNode::poseCallback, this);
        //导航目标点发送话题
        goal_pub = n.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 1000);
        //速度控制话题
        cmd_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);

        start();
        return true;
    }

    /**
     * @brief 有参初始化函数
     *
     * @param master_url
     * @param host_url
     * @return true
     * @return false
     */
    bool QNode::init(const std::string &master_url, const std::string &host_url)
    {
        std::map<std::string, std::string> remappings;
        remappings["__master"] = master_url;
        remappings["__hostname"] = host_url;
        ros::init(remappings, "monitor");
        if (!ros::master::check())
        {
            return false;
        }
        ros::start();
        ros::NodeHandle n;

        cmdVel_sub = n.subscribe<nav_msgs::Odometry>(odom_topic.toStdString(), 200, &QNode::speedCallback, this);
        power_sub = n.subscribe(power_topic.toStdString(), 1000, &QNode::powerCallback, this);

        //机器人位置话题
        pos_sub = n.subscribe(pose_topic.toStdString(), 1000, &QNode::poseCallback, this);
        //导航目标点发送话题
        goal_pub = n.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 1000);
        //速度控制topic
        cmd_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);

        start();
        return true;
    }

    void QNode::disinit()
    {
        if (ros::isStarted())
        {
            ROS_INFO("ROS will shutdown");
            ros::shutdown();
            ros::waitForShutdown();
        }
        this->exit();
    }

    QMap<QString, QString> QNode::get_topic_list()
    {
        ros::master::V_TopicInfo topic_list;
        ros::master::getTopics(topic_list);
        QMap<QString, QString> res;
        for (auto topic : topic_list)
        {
            res.insert(QString::fromStdString(topic.name), QString::fromStdString(topic.datatype));
        }
        return res;
    }

    //机器人位置话题回调函数
    void QNode::poseCallback(const geometry_msgs::PoseWithCovarianceStamped &pos)
    {
        emit position(pos.header.frame_id.data(), pos.pose.pose.position.x,
                      pos.pose.pose.position.y,
                      pos.pose.pose.orientation.z,
                      pos.pose.pose.orientation.w);
    }

    void QNode::powerCallback(const std_msgs::Float32 &message_holder)
    {
        emit power(message_holder.data);
    }

    void QNode::myCallback(const const std_msgs::Float64 &message_holder)
    {
        qDebug() << message_holder.data << endl;
    }

    //发布导航目标点信息
    void QNode::set_goal(QString frame, double x, double y, double z, double w)
    {

        geometry_msgs::PoseStamped goal;
        goal.header.frame_id = frame.toStdString();

        goal.header.stamp = ros::Time::now();
        goal.pose.position.x = x;
        goal.pose.position.y = y;
        goal.pose.orientation.z = z;
        goal.pose.orientation.w = w;
        //发送goal
        goal_pub.publish(goal);
        ros::spinOnce();
    }

    void QNode::speedCallback(const nav_msgs::Odometry::ConstPtr &msg)
    {
        emit speed_x(msg->twist.twist.linear.x);
        emit speed_y(msg->twist.twist.linear.y);
    }

    void QNode::run()
    {
        ros::Rate loop_rate(1);
        while (ros::ok())
        {
            //调用消息处理回调函数
            ros::spinOnce();
            loop_rate.sleep();
        }
    }

    //发布机器人速度控制函数
    //-----------------------------------------------------------------------------
    void QNode::move_base(char k, float speed_linear, float speed_turn)
    {
        std::map<char, std::vector<float>> moveBindings{
            {'i', {1, 0, 0, 0}},
            {'o', {1, 0, 0, -1}},
            {'j', {0, 0, 0, 1}},
            {'l', {0, 0, 0, -1}},
            {'u', {1, 0, 0, 1}},
            {',', {-1, 0, 0, 0}},
            {'.', {-1, 0, 0, 1}},
            {'m', {-1, 0, 0 - 1}},
            {'k', {0, 0, 0, 0}}};
        char key = k;
        //计算机器人行驶方向。
        float x = moveBindings[key][0];
        float y = moveBindings[key][1];
        float z = moveBindings[key][2];
        float th = moveBindings[key][3];

        //计算线速度和角速度
        float speed = speed_linear;
        float turn = speed_turn;

        // update the twist message;
        geometry_msgs::Twist twsit;
        twsit.linear.x = static_cast<double>(x * speed);
        twsit.linear.y = static_cast<double>(y * speed);
        twsit.linear.z = static_cast<double>(z * speed);

        twsit.angular.x = 0;
        twsit.angular.y = 0;
        twsit.angular.z = static_cast<double>(th * turn);

        // publish ti resolve any remaining callbacks
        cmd_pub.publish(twsit);
        ros::spinOnce();
    }
    //-----------------------------------------------------------------------------

    //订阅图像话题，在label上显示
    //-----------------------------------------------------------------------------
    void QNode::Sub_Image(QString topic, int frame_id)
    {
    }
    //-----------------------------------------------------------------------------

    //图像话题回调函数
    //----------------------------------------------------------------------------
    void QNode::imageCallback0(const sensor_msgs::ImageConstPtr &msg)
    {
    }
    void QNode::imageCallback1(const sensor_msgs::ImageConstPtr &msg)
    {
    }
    void QNode::imageCallback2(const sensor_msgs::ImageConstPtr &msg)
    {
    }
    void QNode::imageCallback3(const sensor_msgs::ImageConstPtr &msg)
    {
    }
    //-----------------------------------------------------------------------------

    //图像处理函数，处理gray图，彩色图，以及融合图
    //-----------------------------------------------------------------------------
    QImage QNode::Mat2QImage(cv::Mat const &src)
    {
    }
    //-----------------------------------------------------------------------------

    // logging函数
    void QNode::log(const LogLevel &level, const std::string &msg)
    {
        logging_model.insertRows(logging_model.rowCount(), 1);
        std::stringstream logging_model_msg;
        switch (level)
        {
        case (Debug):
        {
            ROS_DEBUG_STREAM(msg);
            logging_model_msg << "[DEBUG] [" << ros::Time::now() << "]: " << msg;
            break;
        }
        case (Info):
        {
            ROS_INFO_STREAM(msg);
            logging_model_msg << "[INFO] [" << ros::Time::now() << "]: " << msg;
            break;
        }
        case (Warn):
        {
            ROS_WARN_STREAM(msg);
            logging_model_msg << "[INFO] [" << ros::Time::now() << "]: " << msg;
            break;
        }
        case (Error):
        {
            ROS_ERROR_STREAM(msg);
            logging_model_msg << "[ERROR] [" << ros::Time::now() << "]: " << msg;
            break;
        }
        case (Fatal):
        {
            ROS_FATAL_STREAM(msg);
            logging_model_msg << "[FATAL] [" << ros::Time::now() << "]: " << msg;
            break;
        }
        }
        QVariant new_row(QString(logging_model_msg.str().c_str()));
        logging_model.setData(logging_model.index(logging_model.rowCount() - 1), new_row);
        Q_EMIT loggingUpdated(); // used to readjust the scrollbar
    }

} // namespace