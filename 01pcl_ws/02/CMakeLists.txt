cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(c++learning)



set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

find_package(cmake_modules REQUIRED)


add_executable (01 src/01.cpp)
target_link_libraries (01 ${PCL_LIBRARIES})


