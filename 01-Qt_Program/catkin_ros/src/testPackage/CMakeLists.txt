cmake_minimum_required(VERSION 3.5)
project(testPackage)

#自动添加当前源码和编译路径到头文件路径。
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
#使用c++版本
set(CMAKE_CXX_STANDARD 17)
#是否指明c++版本-》是
set(CMAKE_CXX_STANDARD_REQUIRED ON)
######################################################
#通过find_package加载Qt插件
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 编译成可执行文件的代码
set(PROJECT_SOURCES
        cmd_button.cpp
        cmd_button.h
        cmd_button.ui
        main.cpp
)
############################################################
# REQUIRED的作用是当没有find，直接暂停
find_package(catkin REQUIRED
    COMPONENTS
    roscpp
    rospy
    std_msgs
    )
    
# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
# find_package(Qt6 COMPONENTS Widgets REQUIRED)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES testPackage
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)

# 通过include_directories加载头文件
include_directories(
include
${catkin_INCLUDE_DIRS}
)

# 生成可执行文件
 add_executable(testPackage_node 
                ${PROJECT_SOURCES}
                )
# 链接动态链接库
target_link_libraries (testPackage_node
                      ${catkin_LIBRARIES}
                      Qt${QT_VERSION_MAJOR}::Widgets
                      )
