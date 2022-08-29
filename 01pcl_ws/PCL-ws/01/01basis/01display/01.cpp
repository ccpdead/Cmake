#include <iostream>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/cloud_viewer.h>

int main(int argc, char ** argv)
{
    pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGBA>);
    pcl::io::loadPCDFile("./data/pcl_logo.pcd", *cloud);
    pcl::visualization::CloudViewer viewer("Cloud Viewer");
    viewer.showCloud(cloud);

    while(!viewer.wasStopped()){

    }
    return 0;
}
