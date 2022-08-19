#include <pcl/point_cloud.h>
#include <pcl/kdtree/kdtree_flann.h>

#include <iostream>
#include <vector>
#include <ctime>
#include <pcl/visualization/cloud_viewer.h>


int main(int argc, char **argv)
{   
    //用系统时间初始化随机种子
    srand(time(NULL));
    
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);

    //生成点云数据1000个
    cloud->width = 1000;
    //1 表示点云为无序点云
    cloud->height = 1;
    cloud->points.resize(cloud->width * cloud->height);

    //给点云填充数据 0-1023
    for(size_t i =0;i<cloud->points.size(); ++i)
    {
        cloud->points[i].x = 1024.0f * rand() /(RAND_MAX + 1.0f);
        cloud->points[i].y = 1024.0f * rand() /(RAND_MAX + 1.0f);
        cloud->points[i].z = 1024.0f * rand() /(RAND_MAX + 1.0f);
    }
    //创建KDtree的实现类kdtreeflann
    pcl::KdTreeFLANN<pcl::PointXYZ> kdtree;
    //设置搜索空间，把cloud作为输入
    kdtree.setInputCloud(cloud);

    //初始化一个随机的点，作为查询点
    pcl::PointXYZ searchPoint;

    searchPoint.x = 1024.0f * rand() /(RAND_MAX + 1.0f);
    searchPoint.y = 1024.0f * rand() /(RAND_MAX + 1.0f);
    searchPoint.z = 1024.0f * rand() /(RAND_MAX + 1.0f);

    // K nearest neighbor search
    // 方式一：搜索K个最近邻居

    // 创建K和两个向量来保存搜索到的数据
    // K = 10 表示搜索10个临近点
    // pointIdxNKNSearch        保存搜索到的临近点的索引
    // pointNKNSquaredDistance  保存对应临近点的距离的平方
    int K = 5;
    std::vector<int>pointIdxNKNSearch(K);
    std::vector<float>pointNKNSquaredDistance(K);

    std::cout<<"K nearest neighbor search at ("<<searchPoint.x
             <<"    "<<searchPoint.y
             <<"    "<<searchPoint.z
             <<")with K ="<<K<<std::endl;
    
    if(kdtree.nearestKSearch(searchPoint, K, pointIdxNKNSearch, pointNKNSquaredDistance) > 0)
    {
        for(size_t i = 0;i < pointIdxNKNSearch.size(); ++i)
        std::cout<<"    "<<cloud->points[pointIdxNKNSearch[i]].x
                 <<"    "<<cloud->points[pointIdxNKNSearch[i]].y
                 <<"    "<<cloud->points[pointIdxNKNSearch[i]].z
                 <<"(距离平方"<<pointNKNSquaredDistance[i]<<")"<<std::endl;
    }
    // Neighbors within radius search
    // 方式二：通过指定半径搜索
    std::vector<int> pointIDxRadiusSearch;
    std::vector<float>pointRadiusSquaredDistance;

    //创建一个随机【0，256】的半径值
    //float radius = 256.0f * rand() /(RAND_MAX + 1.0f);
    float radius = 100;
    std::cout<<"Neighbors within radius search at (" <<searchPoint.x
             <<"    "<<searchPoint.y
             <<"    "<<searchPoint.z
             <<")with radius ="<<radius<<std::endl;
    if(kdtree.radiusSearch(searchPoint, radius, pointIDxRadiusSearch, pointRadiusSquaredDistance) > 0){
        for(size_t i = 0;i < pointIDxRadiusSearch.size();++i)
        std::cout<<"    "<<cloud->points[pointIDxRadiusSearch[i]].x
                 <<"    "<<cloud->points[pointIDxRadiusSearch[i]].y
                 <<"    "<<cloud->points[pointIDxRadiusSearch[i]].z
                 <<"(距离平方：:"<<pointRadiusSquaredDistance[i]<<")"<<std::endl;
    }
    pcl::visualization::PCLVisualizer viewer("PCL Viewer");
    viewer.setBackgroundColor(0.0, 0.0, 0.5);
    viewer.addPointCloud<pcl::PointXYZ>(cloud, "cloud");

    pcl::PointXYZ originPoint(0.0, 0.0, 0.0);
    //添加从原点到搜索点的线段
    viewer.addLine(originPoint, searchPoint);
    //添加一个以搜索点为圆心，搜索半径为半径的球体
    viewer.addSphere(searchPoint, radius, "sphere", 0);
    //添加一个放大200倍的坐标系
    viewer.addCoordinateSystem(200);

    while(!viewer.wasStopped()){
        viewer.spinOnce();
    }
    return 0;
}