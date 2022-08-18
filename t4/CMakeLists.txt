cmake_minimum_required(VERSION 2.8.3)
project(file)

#指定source文件夹位置和bin位置
# ADD_SUBDIRECTORY(src "bin")

#executable_output_path----》存放目标二进制的位置。
#library_output_path----》存放共享库的位置。
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(SRC_LIST "01.cpp")
set(FILE_OUT "02.cpp")
set(IO "03.cpp")

#${project}_binary_dir----》编译发生的当前目录。
MESSAGE(STATUS"This is BINARY dir" ${FILE_BINARY_DIR})
#${project}_source_dir----》源代码目录。
MESSAGE(STATUS"This is SOURCE dir" ${FILE_SOURCE_DIR})

#$根据源代码在当前工程生成可执行文件。
ADD_EXECUTABLE(file_in src/${SRC_LIST})

ADD_EXECUTABLE(file_put src/${FILE_OUT})

ADD_EXECUTABLE(IO src/${IO})