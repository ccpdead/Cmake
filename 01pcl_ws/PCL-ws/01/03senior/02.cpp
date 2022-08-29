#include <Eigen/Core>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/common/time.h>
#include <pcl/console/print.h>
#include <pcl/features/normal_3d_omp.h>
#include <pcl/features/fpfh_omp.h>
#include <pcl/filters/filter.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/io/pcd_io.h>
#include <pcl/registration/icp.h>
#include <pcl/registration/sample_consensus_prerejective.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/visualization/pcl_visualizer.h>

//定义数据类型
typedef pcl::PointNormal PointNT;
typedef pcl::PointCloud<PointNT> PointCloudT;
typedef pcl::FPFHSignature33 FeatureT;
typedef pcl::FPFHEstimationOMP<PointNT,PointNT,FeatureT> FeatureEstimationT;
typedef pcl::PointCloud<FeatureT> FeatureCloudT;
typedef pcl::visualization::PointCloudColorHandlerCustom<PointNT> ColorHandlerT;

int main(int argc, char **argv)
{
    //实例化必要的数据容器，
    PointCloudT::Ptr object(new PointCloudT);
    PointCloudT::Ptr object_aligned(new PointCloudT);
    PointCloudT::Ptr scene(new PointCloudT);
    FeatureCloudT::Ptr object_features(new FeatureCloudT);
    FeatureCloudT::Ptr scene_features(new FeatureCloudT);

    //get input object and scene
    if(argc!=3)
    {
        pcl::console::print_error("Syntax is: %s object.pcd scene.pcd\n", argv[0]);
        return 1;
    }
    
    //load object and scene
    pcl::console::print_highlight("Loading point clouds.....\n");
    if(pcl::io::loadPCDFile<PointNT> (argv[1],*object)< 0||
       pcl::io::loadPCDFile<PointNT>(argv[2],*scene)<0)
       {
           pcl::console::print_error("Error loading object/scene file!\n");
           return 1;
       }
    //Downsample
    //为了加快处理数度，我们使用pcl的pcl：：VoxelGrid类将对象和场景云的采样率下降至5mm
    pcl::console::print_highlight("Downsampling....\n");
    pcl::VoxelGrid<PointNT> grid;
    const float leaf = 0.005f;

    grid.setLeafSize(leaf, leaf, leaf);
    grid.setInputCloud(object);
    grid.filter (*object);
    grid.setInputCloud(scene);
    grid.filter(*scene);

    //Estimate normals for scene
    pcl::console::print_highlight("Estimating scene normals....\n");
    pcl::NormalEstimationOMP<PointNT, PointNT> nest;
    nest.setRadiusSearch(0.01);
    nest.setInputCloud(scene);
    nest.compute(*scene);

    //Estimate features
    //对于下采样点云中每个点，我们使用pcl中的pcl::FPFHEstimationOMP类来计算用于对齐过程中
    //用于匹配的快速点特征直方图（FPFH）描述符
    pcl::console::print_highlight("Estimationg features...\n");
    FeatureEstimationT fest;
    fest.setRadiusSearch(0.025);

    fest.setInputCloud(object);
    fest.setInputNormals(object);
    fest.compute(*object_features);

    fest.setInputCloud(scene);
    fest.setInputNormals(scene);
    fest.compute(*scene_features);
    //perform alignment
    //sampleconsensusPrerejective 实现有效的RANSAC姿势估计循环
    pcl::console::print_highlight("Starting alignment.....\n");
    pcl::SampleConsensusPrerejective<PointNT, PointNT, FeatureT> align;
    align.setInputCloud(object);
    align.setSourceFeatures(object_features);
    align.setInputTarget(scene);
    align.setTargetFeatures(scene_features);
    align.setMaximumIterations(50000);//Number of RANSAC iterations
    //样本数，在对象和场景之间进行采样的点对应数，至少三个点才能计算姿势
    align.setNumberOfSamples(3);//Number of points to sample for generating/prerejecting a pose
    //对应随机性，我们可以在N个最佳匹配之间随机选择，而不是将每个对象FPFH描述符匹配到场景中最接近的匹配特征，这增加了必要的
    //迭代，也是算法对一场匹配具有鲁棒性
    align.setCorrespondenceRandomness(5);//number of nearest features to use
    //多边形相似度阀值，该直也接近1，则贪婪程度也高，算法变得月块
    align.setSimilarityThreshold(0.9f);//polygonal edge length similarity threshold
    //内在阀值，用于确定变换后的对象点是否正确对齐到最近的场景点
    align.setMaxCorrespondenceDistance(2.5f * leaf);//Inlier threshold
    //正确对齐的点绝对数量是使用inlier阀值确定的
    align.setInlierFraction(0.25f);//required inlier fraction for accepting a pose hypothesis
    {
        //执行对齐配准过程
        pcl::ScopeTime t("Alignment");
        //对齐的对象储存在点云中，
        align.align(*object_aligned);
    }
    if(align.hasConverged())
    {
        //Print results
        //打印可视化结果
        printf("\n");
        Eigen::Matrix4f transformation=align.getFinalTransformation();
        pcl::console::print_info ("    | %6.3f %6.3f %6.3f | \n", transformation (0,0), transformation (0,1), transformation (0,2));
        pcl::console::print_info ("R = | %6.3f %6.3f %6.3f | \n", transformation (1,0), transformation (1,1), transformation (1,2));
        pcl::console::print_info ("    | %6.3f %6.3f %6.3f | \n", transformation (2,0), transformation (2,1), transformation (2,2));
        pcl::console::print_info ("\n");
        pcl::console::print_info ("t = < %0.3f, %0.3f, %0.3f >\n", transformation (0,3), transformation (1,3), transformation (2,3));
        pcl::console::print_info ("\n");
        pcl::console::print_info ("Inliers: %i/%i\n", align.getInliers ().size (), object->size ());

        //show alignment
        pcl::visualization::PCLVisualizer visu("alignment");
        visu.addPointCloud(scene, ColorHandlerT(scene,0.0,255.0,0.0), "scene");
        visu.addPointCloud(object_aligned, ColorHandlerT(object_aligned, 0.0, 0.0, 255), "object_aligned");
        visu.spin();
    }
    else
    {
        pcl::console::print_error("Alignment failed!\n");
        return 1;
    }

    return 0;
}