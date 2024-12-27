#include "user.pb.h"
#include "mprpcapplication.h"
#include "mprpcchannel.h"
#include <iostream>

int main(int argc,char** argv){

    //程序启动以后，想使用mprpc框架来享受rpc服务调用，一定需要先调用框架的初始化函数
    MprpcApplication::Init(argc,argv);

    //演示调用远程发布的rpc方法Login
    fixbug::UserServiceRPC_Stub stub(new MprpcChannel());
    
                     
    return 0;
}