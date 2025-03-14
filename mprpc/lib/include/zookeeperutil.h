#pragma once
#define THREADED
#include <semaphore.h>
#include <zookeeper/zookeeper.h>
#include<string>

//封装的Zk客户端类
class ZkClient{
public:
    ZkClient();
    ~ZkClient();
    //zkclient启动并且连接zkserver
    void Start();
    //在zkserver上根据指定的path创建znode节点,state=0让zooserver创建永久性节点
    void Create(const char * path,const char* data,int datalen,int state=0);
    //根据参数指定的znode节点路径，获取znode节点的值（ip+port）
    std::string GetData(const char* path);
private:
    //zk客户端句柄
    zhandle_t *m_zhandle;
};