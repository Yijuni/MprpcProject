#include <iostream>
#include <string>
#include "mprpcapplication.h"
#include "rpcprovider.h"
#include "friend.pb.h"
#include "logger.h"
#include <vector>
class FriendService:public fixbug::FriendServiceRPC
{
public:
    std::vector<std::string> GetFriendList(uint32_t userid)
    {
        std::cout<<"do GetFriendLists service!"<<std::endl;
        std::vector<std::string> vec;
        vec.push_back("Tom");
        vec.push_back("Alice");
        vec.push_back("Spider man");
        return vec;
    }
    //重写基类方法
    void GetFriendList(google::protobuf::RpcController* controller,
                       const ::fixbug::GetFriendsListRequest* request,
                       ::fixbug::GetFriendsListResponse* response,
                       ::google::protobuf::Closure* done)
    {
        uint32_t user_id = request->userid();
        std::vector<std::string> friendsList = GetFriendList(user_id);
        response->mutable_result()->set_errorcode(0);
        response->mutable_result()->set_errmsg("");
        for(std::string& name:friendsList){
            std::string * p = response->add_friends();
            *p = name;
        }

        done->Run();//执行回调CLosure，这个closure会调用内部注册的回调函数
    }
};

int main(int argc,char** argv){
    LOG_INFO("first log message!\n");
    LOG_ERROR("%s,%s,%d",__FILE__,__FUNCTION__,__LINE__);
    MprpcApplication::Init(argc,argv);
    RpcProvider provider;
    provider.NotifyService(new FriendService());
    provider.Run();
    return 0;
}