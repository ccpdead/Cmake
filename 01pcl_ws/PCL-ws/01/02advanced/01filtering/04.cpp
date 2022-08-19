#include <iostream>
#include <pcl/point_types.h>
#include <pcl/filters/radius_outlier_removal.h>
#include <pcl/filters/conditional_removal.h>
#include <pcl/visualization/pcl_visualizer.h>

typedef pcl::PointXYZ PointType;

void
showPointClouds(const pcl::PointCloud<PointType>::Ptr &cloud, const pcl::PointCloud<PointType>::Ptr &cloud2) {// 创建PCLVisualizer
    pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));

    // 设置背景色为灰色
    viewer->setBackgroundColor(0.05, 0.05, 0.05, 0);

    // 添加一个普通点云 (可以设置指定颜色，也可以去掉single_color参数不设置)
    pcl::visualization::PointCloudColorHandlerCustom<PointType> single_color(cloud, 0, 255, 0);
    viewer->addPointCloud<PointType>(cloud, single_color, "sample cloud");
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 10, "sample cloud");

    // 添加一个第二个点云 (可以设置指定颜色，也可以去掉single_color2参数不设置)
    pcl::visualization::PointCloudColorHandlerCustom<PointType> single_color2(cloud, 255, 0, 0);
    viewer->addPointCloud<PointType>(cloud2, single_color2, "sample cloud 2");
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 10, "sample cloud 2");
    viewer->addCoordinateSystem(1.0);

    while (!viewer->wasStopped()) {
        viewer->spinOnce();
    }
}

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cerr<<"please specify command line arg '-r', or '-c'"<<std::endl;
        exit(0);
    }
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);

    //Fill in the cloud data
    cloud->width = 100;
    cloud->height = 1;
    cloud->points.resize(cloud->height * cloud->width);

    for(size_t i = 0;i<cloud->points.size();++i)
    {
        cloud->points[i].x = 1024 * rand() /(RAND_MAX + 1.0f);
        cloud->points[i].y = 1024 * rand() /(RAND_MAX + 1.0f);
        cloud->points[i].z = 1024 * rand() /(RAND_MAX + 1.0f);
    }
    //strcmp函数比较两个字符串
    if(strcmp(argv[1], "-r") == 0)
    {
        pcl::RadiusOutlierRemoval<pcl::PointXYZ> outrem;//半径离群值删除
        //build th filter
        outrem.setInputCloud(cloud);
        outrem.setRadiusSearch(0.4);
        outrem.setMinNeighborsInRadius(2);
        //aply filter
        outrem.filter(*cloud_filtered);
    }else if(strcmp(argv[1], "-c") == 0)
    {
        //build the condition（条件）
        pcl::ConditionAnd<pcl::PointXYZ>::Ptr range_cond(new pcl::ConditionAnd<pcl::PointXYZ>());
        range_cond->addComparison(pcl::FieldComparison<pcl::PointXYZ>::ConstPtr(
            new pcl::FieldComparison<pcl::PointXYZ>("z", pcl::ComparisonOps::GT, 0.0)));

        range_cond->addComparison(pcl::FieldComparison<pcl::PointXYZ>::ConstPtr(
            new pcl::FieldComparison<pcl::PointXYZ>("z", pcl::ComparisonOps::LT, 0.8)));
        
        //build the filter
        pcl::ConditionalRemoval<pcl::PointXYZ> condrem;
        condrem.setCondition(range_cond);
        condrem.setInputCloud(cloud);
        condrem.setKeepOrganized(true);
        //apply filter
        condrem.filter(*cloud_filtered);
    }else
    {
        std::cerr<<"please specify command line arg '-r' or '-c'"<<std::endl;
    }
    
    std::cerr<<"Cloud before filtering:"<<std::endl;
    for(size_t i =0 ;i<cloud->points.size();++i)
    std::cerr<<"    "<<cloud->points[i].x<<"    "
                <<cloud->points[i].y<<"    "
                <<cloud->points[i].z<<std::endl;
    
    //display pointcloud after filtering
    std::cerr<<"Cloud after filtering:"<<std::endl;
    for(size_t i = 0;i< cloud_filtered->points.size();++i)
    std::cerr<<"    "<<cloud_filtered->points[i].x<<"   "
                <<cloud_filtered->points[i].y<<"   "
                <<cloud_filtered->points[i].z<<std::endl;
    
    showPointClouds(cloud, cloud_filtered);
    return 0;
    
}