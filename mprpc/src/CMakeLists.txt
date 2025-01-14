#当前目录所有文件放入RC_LIST
aux_source_directory(. SRC_LIST)

#如果cmake过程中出现链接错误，那是因为cmake没有检测到新添加的文件，这时候可以用set添加新的文件让其参与编译链接
#但是如果新加的文件太多，可以去build文件夹把文件全删了，再去构建就会检测到新文件
# 添加编译的文件
# set(SRC_LIST mprpcapplication.cc mprpcconfig.cc) 
#生成动态库
add_library(mprpc ${SRC_LIST})
target_link_libraries(mprpc muduo_net muduo_base zookeeper_mt pthread)