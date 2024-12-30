#include "user.pb.h"
#include "mprpcapplication.h"
#include "mprpcchannel.h"
#include <iostream>

int main(int argc,char** argv){

    //程序启动以后，想使用mprpc框架来享受rpc服务调用，一定需要先调用框架的初始化函数
    MprpcApplication::Init(argc,argv);

    //演示调用远程发布的rpc方法Login
    fixbug::UserServiceRPC_Stub stub(new MprpcChannel());
    //rpc方法的请求参数
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");
    //rpc方法的响应
    fixbug::LoginResponse response;
    //rpc方法的调用，同步rpc调用过程 MprpcChannel::callMethod
    stub.Login(nullptr,&request,&response,nullptr);

    //rpc方法调用完成，读调用结果
    if(response.result().errorcode()!=0){
        std::cout<<"rpc login response error:"<<response.result().errmsg()<<std::endl;
    }
    std::cout<<"rpc login response:"<<response.success()<<std::endl;
    return 0;
}