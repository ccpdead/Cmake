#include<pcl/visualization/cloud_viewer.h>
#include<iostream>
#include<pcl/io/io.h>
#include<pcl/io/pcd_io.h>
#include<pcl/features/normal_3d.h>

int main()
{
    //load point cloud
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::io::loadPCDFile("./data/milk_color.pcd",*cloud);
    //estimate normal
    pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);

    //object for normal estimation
    pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> normalEstimation;
    //normalEstimation.setIndices()
    normalEstimation.setInputCloud(cloud);
    //for every point ,use all neighbors in a radius of 3cm
    normalEstimation.setRadiusSearch(0.03);
    // a kd-tree is a data structure that makes searches efficient. mor about it later
    //the normal estimation object will use it to find nearest neighbors
    pcl::search::KdTree<pcl::PointXYZ>::Ptr kdtree(new pcl::search::KdTree<pcl::PointXYZ>);
    normalEstimation.setSearchMethod(kdtree);
    //calculate the normals
    normalEstimation.compute(*normals);

    //visualize normals
    pcl::visualization::PCLVisualizer viewer("pcl viewer");
    viewer.setBackgroundColor(0.0, 0.0, 0.5);
    viewer.addPointCloud<pcl::PointXYZ>(cloud, "cloud");
    // 参数int level=2 表示每n个点绘制一个法向量
    // 参数float scale=0.01 表示法向量长度缩放为0.01倍
    viewer.addPointCloudNormals<pcl::PointXYZ,pcl::Normal>(cloud, normals,2,0.01,"normals");
    while(!viewer.wasStopped())
    {
        viewer.spinOnce();
    }
    return 0;
}