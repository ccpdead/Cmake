cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(test)

# 设置输出根目录为build/Debug
set(OUTPUT_DIRECTORY_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})
# 设置可执行程序输出到build/Debug/bin目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY_ROOT}/bin" CACHE PATH "Runtime directory" FORCE)
# 设置库文件输出到build/Debug/lib目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY_ROOT}/lib" CACHE PATH "Library directory" FORCE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY_ROOT}/lib" CACHE PATH "Archive directory" FORCE)
set(CMAKE_CXX_STANDARD 14)

find_package(PCL REQUIRED
PATHS ~/pcl-1.9.1/install
NO_DEFAULT_PATH)

#包含头文件目录
include_directories(${PCL_INCLUDE_DIRS})
#设置依赖库链接目标
link_directories(${PCL_LIBRARY_DIRS})
#添加预处理器和编译器标志
add_definitions(${PCL_DEFINITIONS})

add_executable (main 04application/05.cpp)
target_link_libraries (main ${PCL_LIBRARIES})
