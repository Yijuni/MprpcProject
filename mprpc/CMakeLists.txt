# 设置cmake最低版本要求和项目名
cmake_minimum_required(VERSION 3.0)
project(mprpc)

#设置项目可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#设置项目库文件输出路径
set(LIBRARY_OUTPUT_DIRECTORY $(PROJECT_SOURCE_DIR)/lib)

#设置项目头文件搜索路径 -I <目录路径>
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/example)
#设置项目库文件搜索路径 -L <目录路径>
link_directories(${PROJECT_SOURCE_DIR}/lib)

#add_subdirectory(src)
add_subdirectory(example)