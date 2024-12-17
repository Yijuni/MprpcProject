set(SRC_LIST userservice.cc ../user.pb.cc)
add_executable(provider ${SRC_LIST})
#给生成的provider链接动态库mprpc和protobuf，target_link_libraries可以为特定的文件链接动态库
target_link_libraries(provider mprpc protobuf)