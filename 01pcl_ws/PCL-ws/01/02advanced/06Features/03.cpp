#include<pcl/point_types.h>
#include<pcl/features/pfh.h>
#include<pcl/io/io.h>
#include<pcl/io/pcd_io.h>
#include<pcl/visualization/cloud_viewer.h>
#include<pcl/features/normal_3d.h>

int main()
{
    //load point cloud
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::io::loadPCDFile("./data/bunny.pcd", *cloud);

    //estimate normal
    pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
    //object for normal estimation
    pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> normalEstimation;
    normalEstimation.setInputCloud(cloud);
    normalEstimation.setRadiusSearch(0.03);

    //A kd-tree is a data structure that makes searches efficient 
    //the normal estimation object will use it to find nearest neighbors
    pcl::search::KdTree<pcl::PointXYZ>::Ptr kdtree(new pcl::search::KdTree<pcl::PointXYZ>);
    normalEstimation.setSearchMethod(kdtree);
    //calculate the normals
    normalEstimation.compute(*normals);

    //create the pfh estimation class and pass the input dataset+normals to if计算PFH直方图
    pcl::PFHEstimation<pcl::PointXYZ, pcl::Normal, pcl::PFHSignature125> pfh;
    pfh.setInputCloud(cloud);
    pfh.setInputNormals(normals);

    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>());
    pfh.setSearchMethod(tree);
    pcl::PointCloud<pcl::PFHSignature125>::Ptr pfhs(new pcl::PointCloud<pcl::PFHSignature125>());

    pfh.setRadiusSearch(0.08);
    pfh.compute(*pfhs);
    unsigned long size = pfhs->points.size();
    for(int j = 0;j<size;++j)
    {
        pcl::PFHSignature125 &signature125 = pfhs->points[j];
        float *h = signature125.histogram;
        printf("%d: %f,%f,%f\n",j,h[1],h[2],h[3]);
    }
    pcl::visualization::PCLVisualizer viewer("pcl viewer");
    viewer.setBackgroundColor(0,0,0.5);
    viewer.addPointCloudNormals<pcl::PointXYZ,pcl::Normal>(cloud, normals, 1, 0.01, "normals");
    while(!viewer.wasStopped())
    {
        viewer.spinOnce();
    }
    return 0;
}