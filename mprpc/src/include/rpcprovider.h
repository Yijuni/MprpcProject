#pragma once
#include "google/protobuf/service.h"
#include "google/protobuf/descriptor.h"
#include "mprpcapplication.h"
#include <string>
#include <functional>
#include <memory>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/net/Buffer.h>
#include <muduo/base/Timestamp.h>
#include <unordered_map>
//框架提供的专门发布rpc服务的网络对象类
class RpcProvider{
public:
    //所都得rpc服务类都是从google的Service类继承而来,所以参数用Service类来接受用户定义的rpcservice
    //用户可以通过此方法发布自己rpcservice
    void NotifyService(google::protobuf::Service *service);
    //启动rpc服务节点，开始提供rpc远程网络调用服务
    void Run();

private:
    //服务器监听连接请求的主loop
    muduo::net::EventLoop m_eventloop;
    //服务信息，保存了某个服务对象指针和对应的服务方法描述器
    struct ServiceInfo
    {
        google::protobuf::Service *m_service;//保存服务对象指针
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> m_methodMap;//保存服务方法名字及其对应描述器 
    };
    std::unordered_map<std::string,ServiceInfo> m_serviceMap;
    //新的socket链接回调
    void OnConnection(const muduo::net::TcpConnectionPtr&);
    //已建立用户读写回调
    void OnMessage(const muduo::net::TcpConnectionPtr&,muduo::net::Buffer*,muduo::Timestamp);
    //Closure回调操作,用于序列化rpc的response和网络发送
    void SendRpcResponse(const muduo::net::TcpConnectionPtr&,google::protobuf::Message*);

};