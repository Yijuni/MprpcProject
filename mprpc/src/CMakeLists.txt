#当前目录所有文件放入RC_LIST
aux_source_directory(. SRC_LIST)
#生成动态库
add_library(mprpc SHARED ${SRC_LIST})