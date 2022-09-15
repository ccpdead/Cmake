#ifndef QNODE_H
#define QNODE_H

#include <ros/ros.h>
#include <QThread>
#include <QLabel>
#include <QStringListModel>
#include <QImage>
#include <QSettings>

#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Float32.h>
#include <geometry_msgs/Twist.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/image_encodings.h>
#include <map>

namespace monitor
{
	/**********************************class********************************/
	class QNode : public QThread
	{
		Q_OBJECT
	public:
		QNode(int argc, char **argv);
		virtual ~QNode();
		bool init();
		bool init(const std::string &master_url, const std::string &host_url);
		void disinit();
		void move_base(char k, float speed_linear, float speed_trun);
		void set_goal(QString frame, double x, double y, double z, double w);
		void Sub_Image(QString topic, int frame_id);
		QMap<QString, QString> get_topic_list();
		void run();

		// logging

		enum LogLevel
		{
			Debug,
			Info,
			Warn,
			Error,
			Fatal
		};
		QStringListModel *loggingModel() { return &logging_model; };
		void log(const LogLevel &level, const std::string &msg);
	Q_SIGNALS:
		void loggingUpdated();
		void rosShutdown();
		void speed_x(double x);
		void speed_y(double y);
		void power(float p);
		void Master_shutdown();
		void Show_Image(int, QImage);
		void position(QString frame, double x, double y, double z, double w);

	private:
		int init_argc;
		char **init_argv;
		ros::Publisher chatter_publisher;
		ros::Subscriber cmdVel_sub;
		ros::Subscriber chatter_subscriber;
		ros::Subscriber pos_sub;
		ros::Subscriber power_sub;
		ros::Publisher goal_pub;
		ros::Publisher cmd_pub;
		
		QStringListModel logging_model;

		//图像订阅
		image_transport::Subscriber image_sub0;
		image_transport::Subscriber image_sub1;
		image_transport::Subscriber image_sub2;
		image_transport::Subscriber image_sub3;
		//图像format
		QString video0_format;
		QString video1_format;
		QString video2_format;
		QString video3_format;
		QString odom_topic;
		QString power_topic;
		QString pose_topic;
		QString power_max;
		QString power_min;
		QImage Mat2QImage(cv::Mat const &src);

		void poseCallback(const geometry_msgs::PoseWithCovarianceStamped &pos);
		void speedCallback(const nav_msgs::Odometry::ConstPtr &msg);
		void powerCallback(const std_msgs::Float32 &message_holder);

		void imageCallback0(const sensor_msgs::ImageConstPtr &msg);
		void imageCallback1(const sensor_msgs::ImageConstPtr &msg);
		void imageCallback2(const sensor_msgs::ImageConstPtr &msg);
		void imageCallback3(const sensor_msgs::ImageConstPtr &msg);
		
		void myCallback(const std_msgs::Float64 &message_holder);
	};

} // namespace
#endif // QNODE_H
