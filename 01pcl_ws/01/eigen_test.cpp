#include <Eigen/Eigen>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <Eigen/Eigenvalues>
#include <iostream>

int 
main(int argc,char argv)
{
    Eigen::Matrix3d rotation_matrix;
    rotation_matrix << 0.707107, -0.707107, 0, 0.707107, 0.707107, 0, 0, 0, 1;;
    // Eigen::AngleAxisd rotation_vector;
    // rotation_vector.fromRotationMatrix(rotation_matrix);
    Eigen::Vector3d euler_angles = rotation_matrix.eulerAngles(2, 1, 0); 
    std::cout << "yaw(z) pitch(y) roll(x) = " << euler_angles.transpose() << std::endl;

}