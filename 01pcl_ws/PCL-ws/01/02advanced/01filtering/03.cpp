/* 离群点移除 *¶
激光扫描通常会生成不同点密度的点云数据集。
此外，测量误差会导致稀疏的异常值，从而进一步破坏结果。
这会使局部点云特征（例如表面法线或曲率变化）的估计复杂化，从而导致错误的值，
进而可能导致点云配准失败。通过对每个点的邻域进行统计分析，
并对不符合特定条件的部分进行修整，可以解决其中一些不规则现象。

稀疏离群值的消除基于输入数据集中点到邻居距离的分布的计算。
对于每个点，我们计算从它到所有相邻点的平均距离。
通过假设结果分布是具有均值和标准差的高斯分布，
可以将其平均距离在由全局距离均值和标准差定义的区间之外的所有点视为离群值并从数据集中进行修剪。 下图显示了稀疏离群值分析和删除的效果：
原始数据集显示在左侧，结果数据集显示在右侧。数据集图显示了滤波前后每个点的邻域中平均K最近邻距离。 */

#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/statistical_outlier_removal.h>

int main(int argc, char** argv)
{
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);

    //从文件读取点云
    pcl::PCDReader reader;
    reader.read<pcl::PointXYZ>("/home/liutong/pcl/test/table_scene_lms400.pcd", *cloud);
    
    std::cerr<<"Cloud before filterig:"<<std::endl;
    std::cerr<<*cloud<<std::endl;

    //创建过滤器，每个点分析计算时考虑最近邻居个数为50个；
    //设置标准差阀值为1，这意味着所有距离查询点的平均距离标准差均大于1个标准偏差的所有点将被标记为离群值并删除。
    //计算输出并将储存放在cloud-filtered中
    
    //Create the filtering object
    pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor;
    sor.setInputCloud(cloud);
    //设置平均距离估计的最近距离数量K
    sor.setMeanK(50);
    //设置标准差异阀值系数
    sor.setStddevMulThresh(1.0);
    //执行过滤
    sor.filter(*cloud_filtered);

    std::cerr<<"Cloud after filtering: "<<std::endl;
    std::cerr<<*cloud_filtered<<std::endl;
    //将保留下来的点保存到文件中
    pcl::PCDWriter writer;
    writer.write<pcl::PointXYZ>("/home/liutong/pcl/test/table_scene_lms400_inliers.pcd", *cloud_filtered, false);

    //使用相同的过滤器，但对输出结果取反，将被过滤的点另外储存。
    sor.setNegative(true);
    sor.filter(*cloud_filtered);
    writer.write<pcl::PointXYZ>("/home/liutong/pcl/test/table_scene_lms400_outliers.pcd", *cloud_filtered, false);

    return 0;
}