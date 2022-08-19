cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(PCL_Learning)

find_package(PCL REQUIRED
PATHS ~/pcl-19
NO_DEFAULT_PATH)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

find_package(cmake_modules REQUIRED)
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})
add_definitions(${EIGEN_DEFINITIONS})

add_executable (main main.cpp)
target_link_libraries (main ${PCL_LIBRARIES})

add_executable (view_pcl main-5.cpp)
target_link_libraries (view_pcl ${PCL_LIBRARIES})

add_executable (eigen_test eigen_test.cpp)
target_link_libraries (eigen_test ${PCL_LIBRARIES})