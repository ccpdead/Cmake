#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>

int main(int argc, char **argv)
{
    pcl::PCLPointCloud2::Ptr cloud(new pcl::PCLPointCloud2());
    pcl::PCLPointCloud2::Ptr cloud_filtered (new pcl::PCLPointCloud2());

    //从文件读取点云图
    //fill in the cloud data
    pcl::PCDReader reader;
    //Replace the path below with the path where you saved your file
    reader.read("/home/liutong/pcl/test/table_scene_lms400.pcd", *cloud);

    std::cerr<<"Pointcloud before filtering:"<<cloud->width * cloud->height
    <<"date points ("<<pcl::getFieldsList(*cloud)<<").";
    //创建一个长宽分别是1cm的体素过滤器，cloud作为输入数据，cloud-filtered作为输出数据
    float leftSize = 0.01f;
    //Create the filtering object
    pcl::VoxelGrid<pcl::PCLPointCloud2> sor;
    sor.setInputCloud(cloud);
    sor.setLeafSize(leftSize, leftSize, leftSize);
    sor.filter(*cloud_filtered);

    std::cerr<<"Pointcloud ager filtering:"<<cloud_filtered->width * cloud_filtered->height
    <<"date points ("<<pcl::getFieldsList(*cloud_filtered)<<").";
    //将结果输出到文件
    pcl::PCDWriter writer;
    writer.write("/home/liutong/pcl/test/table_scene_lms400_downsampled.pcd", *cloud_filtered);
    return 0;
}