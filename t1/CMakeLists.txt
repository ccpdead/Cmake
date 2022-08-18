cmake_minimum_required(VERSION 2.8.3)
project(HELLO)

#指定source文件夹位置和bin位置
# ADD_SUBDIRECTORY(src "bin")

#executable_output_path----》存放目标二进制的位置。
#library_output_path----》存放共享库的位置。
#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(SRC_LIST "main.c")

#${project}_binary_dir----》编译发生的当前目录。
MESSAGE(STATUS"This is BINARY dir" ${HELLO_BINARY_DIR})
#${project}_source_dir----》源代码目录。
MESSAGE(STATUS"This is SOURCE dir" ${HELLO_SOURCE_DIR})

#$根据源代码在当前工程生成可执行文件。
ADD_EXECUTABLE(hello src/${SRC_LIST})

#安装文件
install(
  FILES COPYRIGHT README DESTINATION share/doc/cmake/t1)

#安装脚本文件
install(
  PROGRAMS runhello.sh DESTINATION bin
)
#安装文件
install(
  DIRECTORY doc/ DESTINATION share/doc/cmake/t1
)
