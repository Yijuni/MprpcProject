#include <iostream>
#include <string>

#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
 /**
  *UserService是个本地服务，提供了两个进程内的本地方法，Login和GetFriendLists
  *你想把你的本地服务变成远程服务就得写proto并编译，然后本地服务继承生成的RPC服务类，重写他的虚函数
 */

class UserService:public fixbug::UserServiceRPC //使用在RPC服务发布端的，RPC服务的提供者
{
public:
    bool Login(std::string name,std::string pwd){
        std::cout<<"doing local service : Login"<<std::endl;
        std::cout<<"name: "<<name<<" pwd:"<<pwd<<std::endl;
        return true;
    }

    /**
     * 重写基类UserServiceRPC的虚函数，下面这些方法都是框架直接调用的,服务消费者调用服务后，经过网络传输到达服务提供者
     *进而框架会调用服务提供者的RPC方法，进而调用本地方法进行具体业务处理，不用关心怎么进行消息传输、怎么序列化反序列化的
     *只需要填好request和response,框架会自动进行处理
    */
    void Login(google::protobuf::RpcController* controller,const fixbug::LoginRequest* request,
            fixbug::LoginResponse* response,google::protobuf::Closure* done)
    {
        //框架给业务上报了请求参数LoginRequest，业务取出相应数据调用本地业务Login()
        std::string name = request->name();//protobuf 已经帮我们把字节流解析出来了，直接用request就好了
        std::string pwd = request->pwd();
        
        bool login_result = Login(name,pwd);//本地业务
        
        //把响应写入response 错误码，错误消息，返回值
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errmsg("");
        code->set_errorcode(0);
        response->set_success(login_result);

        //执行回调操作,把你填入的response序列化通过网络发送回去
        done->Run(); 
    }
};

int main(int argc,char **argv){
    //调用框架初始化操作
    MprpcApplication::Init(argc,argv);

    //把UserService对象发布到rpc站点上,provider是个rpc网络服务对象
    RpcProvider provider;
    provider.NotifyService(new UserService());

    //启动一个rpc服务发布节点，Run以后进程进入阻塞状态，等待远程rpc调用请求
    provider.Run();

    return 0;
}