#include "main_window.h"
#include "ui_main_window.h"
using namespace Qt;

namespace rviz
{
    //构造函数
    main_window::main_window(int argc, char **argv, QWidget *parent) : QMainWindow(parent),
                                                                       ui(new Ui::main_window),
                                                                       qnode(argc, argv)
    {
        ui->setupUi(this);

        initUis();
        initData();

        //读取配置文件
        ReadSettings();

        /*********************
         * logging
         * ******************/
        ui->listView->setModel(qnode.loggingModel());

        addtopic_form = new addtopics();

        //绑定添加rviz话题信号
        connect(addtopic_form, SIGNAL(Topic_choose(QTreeWidgetItem *, QString)), SLOT(slot_choose_topic(QTreeWidgetItem *, QString)));

        //设置自动链接Master
        if (ui->checkBox_remeber_settings->isChecked())
        {
            on_button_connect_clicked();
        }

        //链接connect
        connections();
    }

    //析构函数
    main_window::~main_window()
    {
        if (base_cmd)
        {
            delete base_cmd;
            base_cmd = nullptr;
        }
        if (map_rviz_)
        {
            delete map_rviz_;
            map_rviz_ = nullptr;
        }
    }

    //初始化UI
    void main_window::initUis()
    {
    }

    //初始化ui数据，
    void main_window::initData()
    {
        m_mapRvizDisplays.insert("Axes", RVIZ_DISPLAY_AXES);
        m_mapRvizDisplays.insert("Camera", RVIZ_DISPLAY_CAMERA);
        m_mapRvizDisplays.insert("DepthCloud", RVIZ_DISPLAY_DEPTHCLOUD);
        m_mapRvizDisplays.insert("Effort", RVIZ_DISPLAY_EFFORT);
        m_mapRvizDisplays.insert("FluidPressure", RVIZ_DISPLAY_FLUIDPRESSURE);
        m_mapRvizDisplays.insert("Grid", RVIZ_DISPLAY_GRID);
        m_mapRvizDisplays.insert("GridCells", RVIZ_DISPLAY_GRIDCELLS);
        m_mapRvizDisplays.insert("Group", RVIZ_DISPLAY_GROUP);
        m_mapRvizDisplays.insert("Illuminance", RVIZ_DISPLAY_ILLUMINANCE);
        m_mapRvizDisplays.insert("Image", RVIZ_DISPLAY_IMAGE);
        m_mapRvizDisplays.insert("InterativerMarker", RVIZ_DISPLAY_INTERATIVEMARKER);
        m_mapRvizDisplays.insert("LaserScan", RVIZ_DISPLAY_LASERSCAN);
        m_mapRvizDisplays.insert("Map", RVIZ_DISPLAY_MAP);
        m_mapRvizDisplays.insert("Marker", RVIZ_DISPLAY_MARKER);
        m_mapRvizDisplays.insert("MarkerArray", RVIZ_DISPLAY_MARKERARRAY);
        m_mapRvizDisplays.insert("Odometry", RVIZ_DISPLAY_ODOMETRY);
        m_mapRvizDisplays.insert("Path", RVIZ_DISPLAY_PATH);
        m_mapRvizDisplays.insert("PointCloud", RVIZ_DISPLAY_POINTCLOUD);
        m_mapRvizDisplays.insert("PointCloud2", RVIZ_DISPLAY_POINTCLOUD2);
        m_mapRvizDisplays.insert("PointStamped", RVIZ_DISPLAY_POINTSTAMPED);
        m_mapRvizDisplays.insert("Polygon", RVIZ_DISPLAY_POLYGON);
        m_mapRvizDisplays.insert("Pose", RVIZ_DISPLAY_POSE);
        m_mapRvizDisplays.insert("PoseArray", RVIZ_DISPLAY_POSEARRAY);
        m_mapRvizDisplays.insert("PoseWithCovariance", RVIZ_DISPLAY_POSEWITHCOVARIANCE);
        m_mapRvizDisplays.insert("Range", RVIZ_DISPLAY_RANGE);
        m_mapRvizDisplays.insert("RelativeHumidity", RVIZ_DISPLAY_RELATIVEHUMIDITY);
        m_mapRvizDisplays.insert("RobotModel", RVIZ_DISPLAY_ROBOTMODEL);
        m_mapRvizDisplays.insert("TF", RVIZ_DISPLAY_TF);
        m_mapRvizDisplays.insert("Temperature", RVIZ_DISPLAY_TEMPERATURE);
        m_mapRvizDisplays.insert("WrenchStamped", RVIZ_DISPLAY_WRENCHSTAMPED);
    }

    //订阅video话题
    void main_window::initVideos()
    {
    }

    //显示image槽函数
    void main_window::slot_show_image(int frame_id, QImage image)
    {
    }

    void main_window::initRviz()
    {
    }

    void main_window::RvizGetModel(QAbstractItemModel *model)
    {
    }

    void main_window::connections()
    {
    }

    void main_window::slot_setting_frame()
    {
    }

    void main_window::slot_position_change(QString frame, double x, double y, double z, double w)
    {
    }

    void main_window::slot_set_return_point()
    {
    }

    void main_window::slot_return_point()
    {
    }

    void main_window::slot_set_2D_Goal()
    {
    }

    void main_window::slot_set_2D_Pos()
    {
    }

    void main_window::slot_set_select()
    {
    }

    void main_window::slot_choose_topic(QTreeWidgetItem *choose, QString name)
    {
    }

    //检查重名
    QString main_window::JudgeDisplayNewName(QString name)
    {
    }

    //左工具栏索引改变
    void main_window::slot_tab_manage_currentChanged(int index)
    {
    }

    //右工具栏索引改变
    void main_window::slot_tab_Widget_currentChanged(int index)
    {
    }

    //速度控制相关按钮处理曹函数
    void main_window::slot_cmd_control()
    {
    }

    //滑动条处理槽函数
    void main_window::on_Slider_raw_valueChanged(int v)
    {
    }

    //滑动条处理槽函数
    void main_window::on_SLider_linear_valueChanged(int v)
    {
    }

    //快捷指令删除按钮
    void main_window::quick_cmd_remove()
    {
    }

    void main_window::quick_cmd_add()
    {
    }

    //向treewidget添加快捷指令
    void main_window::add_quick_cmd(QString name, QString val)
    {
    }

    //快捷指令按钮处理槽函数
    void main_window::quick_cmds_check_change(int state)
    {
    }

    //执行一些命令的回显
    void main_window::cmd_output()
    {
    }

    //执行一些命令的错误回显
    void main_window::cmd_error_output()
    {
    }

    /************************************
     * Implementation slots
     * *********************************/

    void main_window::showNoMasterMessage()
    {
    }

    void main_window::inform(QString strdata)
    {
    }

    void main_window::AskInform(QString strdata)
    {
    }

    void main_window::initTopicList()
    {
    }

    void main_window::refreashTopicList()
    {
    }

    void main_window::slot_resShutdown()
    {
    }

    void main_window::slot_power(float p)
    {
    }

    void main_window::slot_speed_x(double x)
    {
    }

    void main_window::slot_speed_y(double x)
    {
    }

    void main_window::on_checkbox_use_environment_stateChanged(int state)
    {
    }

    void main_window::updateLoggingView()
    {
    }

    void main_window::on_actionAbout_triggered()
    {
    }

    void main_window::ReadSettings()
    {
    }

    void main_window::WriteSettings()
    {
    }

    void main_window::closeEvent(QCloseEvent *event)
    {
    }

    void main_window::on_pushButton_remove_topic_clicked()
    {
    }

    void main_window::on_pushButton_rename_topic_clicked()
    {
    }

    void main_window::on_treeView_rvizDisplayTree_clicked(const QModelIndex &index)
    {
    }

    void main_window::on_button_connect_clicked()
    {
    }

    //断开ROS
    void main_window::on_button_disconnect_clicked()
    {
    }

    //读取RVIZ 配置文件
    void main_window::on_pushButton_rvizReadDisplaySet_clicked()
    {
    }

    //保存RVIZ 配置文件
    void main_window::on_pushButton_rvizSaveDisplaySet_clicked()
    {
    }

    QString main_window::getUserName()
    {
    }

} // namespace
