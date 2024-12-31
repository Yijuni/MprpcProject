# set(SRC_LIST calluserservice.cc ../user.pb.cc)
set(SRC_LIST callfriendservice.cc ../friend.pb.cc)
add_executable(consumer ${SRC_LIST})
#给生成的provider链接动态库mprpc和protobuf，target_link_libraries可以为特定的文件链接动态库
target_link_libraries(consumer mprpc protobuf)