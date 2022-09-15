cmake_minimum_required(VERSION 3.0.2)
project(rviz_tree)
#自动添加当前源码和编译器路径到头头文件路径
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#设定使用C++的版本
set(CMAKE_CXX_STANDARD 17)

#谁否指明CPP版本
set(CMAKE_CXX_STANDARD_REVISION ON)



find_package(catkin REQUIRED
COMPONENTS
roscpp
rospy
std_msgs
rviz)


#加载Qt插件
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

#编译成可执行文件的代码
set(PROJECT_SOURCES

src/main.cpp

src/main_window.cpp
src/main_window.h
src/main_window.ui

# src/settings.cpp
# src/settings.h
# src/settings.ui

# src/addtopics.cpp
# src/addtopics.h
# src/addtopics.ui

# src/qnode.cpp
# src/qnode.h
)
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES rviz_tree
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)


include_directories(
include
${catkin_INCLUDE_DIRS}
)

#生成可执行文件
add_executable(${PROJECT_NAME}_node
              ${PROJECT_SOURCES}
              )


#链接动态链接库
target_link_libraries(${PROJECT_NAME}_node
                      ${catkin_LIBRARIES}
                      Qt${QT_VERSION_MAJOR}::Widgets
                      )
