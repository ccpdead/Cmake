#include <pcl/range_image/range_image.h>
#include <pcl/io/pcd_io.h>
#include<pcl/visualization/pcl_visualizer.h>
#include <pcl/visualization/range_image_visualizer.h>

int main(int argc, char** argv)
{
    pcl::PointCloud<pcl::PointXYZ>::Ptr pointCloudPtr(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ>& pointCloud = *pointCloudPtr;
    //创建一个矩形形状的点云
    for(float y = -0.5f;y< 0.5f;y+=0.01f)
    {
        for(float z = -0.5f;z<=0.5f;z+=0.01f)
        {
            pcl::PointXYZ point;
            point.x = 2.0f -y;
            point.y = y;
            point.z = z;
            pointCloud.points.push_back(point);
        }
    }
    pointCloud.width = (uint32_t) pointCloud.points.size();
    pointCloud.height = 1;

    //根据之前得到的点云图，通过1deg分辨率生成深度图
    float angularResolution = (float)(1.0f*(M_PI/180.0f));//弧度1
    float maxAngleWidth = (float)(360.0f * (M_PI/180.0f));//弧度360
    float maxAngleHeight=(float)(180.0f*(M_PI/180.0f));//弧度180
    Eigen::Affine3f sensorPose = (Eigen::Affine3f)Eigen::Translation3f(0.0f, 0.0f, 0.0f);//采集位置
    pcl::RangeImage::CoordinateFrame coordinate_frame = pcl::RangeImage::CAMERA_FRAME;//相机坐标系
    float noiseLevel = 0.00;
    float minRange = 0.0f;
    int borderSize = 1;

    boost::shared_ptr<pcl::RangeImage> range_image_ptr(new pcl::RangeImage);
    pcl::RangeImage& rangeImage = *range_image_ptr;
    rangeImage.createFromPointCloud(pointCloud, angularResolution, maxAngleWidth, maxAngleHeight,
    sensorPose, coordinate_frame, noiseLevel, minRange, borderSize);

    std::cout<<rangeImage<<"\n";

    //open 3D viewer and add point cloud
    pcl::visualization::PCLVisualizer viewer("3D viewer");
    viewer.setBackgroundColor(1,1,1);
    //添加深度图点云
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointWithRange> range_image_color_handler(range_image_ptr, 0,0,0);
    viewer.addPointCloud(range_image_ptr, range_image_color_handler,"range_image");
    viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 4, "range_image");


    //添加原始点云
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> org_image_color_handler(pointCloudPtr, 255,0,0);
    viewer.addPointCloud(pointCloudPtr, org_image_color_handler, "orginal_image");
    viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "orginal_image");

    viewer.initCameraParameters();
    viewer.addCoordinateSystem(1.0);

    while(!viewer.wasStopped())
    {
        viewer.spinOnce();
        pcl_sleep(0.01);

    }
    return 0;

}