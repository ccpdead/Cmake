#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_cloud.h>
#include <pcl/console/parse.h>
#include <pcl/common/transforms.h>
#include <pcl/visualization/pcl_visualizer.h>

int main(int argc, char **argv)
{
    //load file
    pcl::PointCloud<pcl::PointXYZ>::Ptr source_cloud(new pcl::PointCloud<pcl::PointXYZ>());
    pcl::io::loadPCDFile("/home/liutong/pcl/test/bunny.pcd", *source_cloud);
/*     if(pcl::io::loadPCDFile(argv[1], *source_cloud )< 0){
        std::cout<<"Error loading point cloud"<<argv[1]<<std::endl<<std::endl;
        return -1;
    } */

/* Reminder: how transformation matrices work :

            |-------> This column is the translation
    | 1 0 0 x |  \
    | 0 1 0 y |   }-> The identity 3x3 matrix (no rotation) on the left
    | 0 0 1 z |  /
    | 0 0 0 1 |    -> We do not use this line (and it has to stay 0,0,0,1)

    METHOD #1: Using a Matrix4f
    This is the "manual" method, perfect to understand but error prone !
*/

    Eigen::Matrix4f transform_1 = Eigen::Matrix4f::Identity();
// Define a rotation matrix (see https://en.wikipedia.org/wiki/Rotation_matrix)

    float theta = M_PI /4;// The angle of rotation in radians
    transform_1(0, 0) = cos(theta);
    transform_1(0, 1) = -sin(theta);
    transform_1(1, 0) -sin(theta);
    transform_1(1, 1) = cos(theta);

//Define a translation of 2.5 meters on the x axis
    transform_1(0, 3) = 2.5;

//Print the transformation
    printf("method #1 : using a Matrix4f\n");
    std::cout<<transform_1<<std::endl;

/*  METHOD #2: Using a Affine3f
    This method is easier and less error prone
*/
    Eigen::Affine3f transform_2 = Eigen::Affine3f::Identity();

//Define a translation of 0.8 meters on the x axis
    transform_2.translation()<<0.8, 0.0, 0.0;

//The same rotation matrix as before : theta radians arround Z axis
    transform_2.rotate(Eigen::AngleAxisf(theta, Eigen::Vector3f::UnitZ()));

//Print the transformation
    printf("\nMethod #2 : using an Affine3f\n");
    std::cout<<transform_2.matrix()<<std::endl;

//Executing the transformation
    pcl::PointCloud<pcl::PointXYZ>::Ptr transformed_cloud(new pcl::PointCloud<pcl::PointXYZ>());
//You can either apply transform_1 or transform_2; they are same
    pcl::transformPointCloud(*source_cloud, *transformed_cloud, transform_2);

//Visualization
    printf("\nPoint cloud colors: white = original point cloud\n"
    "                                red = transformed point cloud\n");
    pcl::visualization::PCLVisualizer viewer("Matrix transformation example");

//Define R, G, B colors for the point cloud
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ>source_cloud_color_handler(source_cloud, 255, 255, 255);

//We add the point cloud to the viewer and pass the color handler
    viewer.addPointCloud(source_cloud, source_cloud_color_handler, "original_cloud");

    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> transformed_cloud_color_handler(transformed_cloud, 230, 20, 20);

    viewer.addPointCloud(transformed_cloud, transformed_cloud_color_handler, "transformed_cloud");
    //设置坐标系系统
    viewer.addCoordinateSystem(0.1, "cloud", 0);
    //设置背景色
    viewer.setBackgroundColor(0.05, 0.05, 0.05, 0);
    //设置渲染属性（点大小）
    viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "original_cloud");
    //设置渲染属性（点大小）
    viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "transformed_cloud");

    while(!viewer.wasStopped()){
        viewer.spinOnce();
    }
    return 0;
}