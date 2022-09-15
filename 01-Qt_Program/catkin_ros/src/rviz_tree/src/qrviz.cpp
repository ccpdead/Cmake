#include "qrviz.h"

qrviz::qrviz(QVBoxLayout* layout, QString node_name)
{
    nullmap.clear();
    this->layout = layout;
    this->nodename = node_name;

    //创建rviz容器
    render_panel_ = new rviz::RenderPanel;
    //向layout中添加widget
}
