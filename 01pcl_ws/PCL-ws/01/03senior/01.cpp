#include<iostream>
#include<thread>
#include<pcl/io/pcd_io.h>
#include<pcl/point_types.h>

#include<pcl/registration/ndt.h>//ndt标准头文件
#include<pcl/filters/approximate_voxel_grid.h>//滤波头文件

#include<pcl/visualization/pcl_visualizer.h>
using namespace std::chrono_literals;

int main(int argc,char**argv)
{
    //lading first scan of room.
    pcl::PointCloud<pcl::PointXYZ>::Ptr target_cloud(new pcl::PointCloud<pcl::PointXYZ>);
    if(pcl::io::loadPCDFile<pcl::PointXYZ>("./data/room_scan1.pcd",*target_cloud) == -1) 
    {
        PCL_ERROR("cloudn,t read file room_scan\n");
        return -1;
    }
    std::cout<<" Loaded "<<target_cloud->size()<<" data points from room_scan1.pcd"<<std::endl;

    //loading second scan of room from new perspective
    pcl::PointCloud<pcl::PointXYZ>::Ptr input_cloud(new pcl::PointCloud<pcl::PointXYZ>);
    if(pcl::io::loadPCDFile<pcl::PointXYZ>("./data/room_scan2.pcd",*input_cloud) == -1) 
    {
        PCL_ERROR("cloudn,t read file room_scan\n");
        return -1;
    }
    std::cout<<" Loaded "<<input_cloud->size()<<" data points form room_scan2.pcd"<<std::endl;
    //以上代码加载两个PCD文件到共享指针，配准操作是完成原点云到目标点云坐标系变换矩阵的估算
    //filtering input scan to roughly 10% of original size to increase speed of registration
    //过滤输入扫描到原始尺寸的10%左右，以提高配准速度
    pcl::PointCloud<pcl::PointXYZ>::Ptr filtered_cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::ApproximateVoxelGrid<pcl::PointXYZ> approximate_voxel_filter;//体素网格过滤器
    approximate_voxel_filter.setLeafSize(0.2, 0.2, 0.2);
    approximate_voxel_filter.setInputCloud(input_cloud);
    approximate_voxel_filter.filter(*filtered_cloud);
    std::cout<<"filtered cloud contains"<<filtered_cloud->size()
             <<"data points from room_scan2.pcd"<<std::endl;
    //以上代码将过滤输入点云到10%的院士大小，目标点云target-cloud不需要进行滤波操作，因为NDT算法在目标点对应的体素网格数据
    //结构计算时，不使用但个点，而是使用体素的点，以做过降采样处理

    //Initializing Normal Distributions transform
    //初始化正态分布变换
    pcl::NormalDistributionsTransform<pcl::PointXYZ, pcl::PointXYZ> ndt;
    
    //Setting scal dependent NDT parameters
    //设置与尺度相关的NDT参数
    //Setting minimum transformation difference for termination condition
    //设定终止条件的最小变换差值。
    ndt.setTransformationEpsilon(0.01);
    //setting maximum step size for More thuente line search
    ndt.setStepSize(0.1);//为mor-thuente线搜索设置最大步长
    //setting resolution of NDT grid structure(VoxelGridCovariance)
    ndt.setResolution(1.0);//设置NDT网络结构分辨率

    //setting max number of registration iterations
    ndt.setMaximumIterations(35);//设置匹配迭代最大次数

    //setting point cloud to be aligned
    ndt.setInputSource(filtered_cloud);//设置过滤后输入点云
    //setting point cloud to be aligned to
    ndt.setInputTarget(target_cloud);//设置目标点云

    //set initial alignment estimate found using robot odometry
    //设置使用机器人测距法得到的初始变换矩阵
    Eigen::AngleAxisf init_rotation(0.6931, Eigen::Vector3f::UnitZ());
    Eigen::Translation3f init_translation(1.79387, 0, 0);
    Eigen::Matrix4f init_guess = (init_translation * init_rotation).matrix();

    //calculation required rigid transform to align the input cloud to the target cloud
    //计算所需的刚体变换，保证输入云与目标云对齐
    pcl::PointCloud<pcl::PointXYZ>::Ptr output_cloud(new pcl::PointCloud<pcl::PointXYZ>);
    ndt.align(*output_cloud, init_guess);

    std::cout<<"normal distributions transform has converged"<<ndt.hasConverged()
             <<" score: "<<ndt.getFitnessScore()<<std::endl;

    //transforming unfiltered input cloud using found transform
    //使用找到的变换矩阵，来对未过滤的输入云进行变换
    pcl::transformPointCloud(*input_cloud, *output_cloud, ndt.getFinalTransformation());

    //saving transformed input cloud
    //保存变换后的输入云
    pcl::io::savePCDFile("room_scan2_transformed.pcd", *output_cloud);

    //initializing point cloud visualizer
    pcl::visualization::PCLVisualizer::Ptr viewer_final(new pcl::visualization::PCLVisualizer("3D viewer"));
    viewer_final->setBackgroundColor(0,0,0);

    //coloring and visualizing traget cloud(red)
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> target_color(target_cloud, 255, 0, 0);
    viewer_final->addPointCloud<pcl::PointXYZ>(target_cloud, target_color, "target cloud");
    viewer_final->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE,1,"target cloud");

    //coloring and visualizing transformed input input cloud(green)
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> output_color(target_cloud, 0, 255, 0);
    viewer_final->addPointCloud<pcl::PointXYZ>(output_cloud, output_color, "output cloud");
    viewer_final->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE,1,"output cloud");

    //starting visualizer
    viewer_final->addCoordinateSystem(1.0, "global");
    viewer_final->initCameraParameters();

    //wait until visualizer window is closed
    while(!viewer_final->wasStopped())
    {
        viewer_final->spinOnce(100);
        std::this_thread::sleep_for(100ms);
    }
    
    return 0;
}