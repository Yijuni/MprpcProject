#pragma once
#include "google/protobuf/service.h"
//框架提供的专门发布rpc服务的网络对象类
class RpcProvider{
public:
    //所都得rpc服务类都是从google的Service类继承而来,所以参数用Service类来接受用户定义的rpcservice
    //用户可以通过此方法发布自己rpcservice
    void NotifyService(google::protobuf::Service *service);
    //启动rpc服务节点，开始提供rpc远程网络调用服务
    void Run();
};