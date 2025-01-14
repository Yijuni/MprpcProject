#include <zookeeperutil.h>
#include "mprpcapplication.h"
#include <zookeeper/recordio.h>
//全局的watcher观察器 zkserver给zkclient通知
void global_watcher(zhandle_t *zh, int type,int state, const char *path,void *watcherCtx){
    if(type == ZOO_SESSION_EVENT){//回调的消息类型和会话相关
        if(state==ZOO_CONNECTED_STATE)//zkclient和zkserver连接成功
        {
            sem_t *sem = (sem_t*)zoo_get_context(zh);//从指定的句柄上获取信号量
            sem_post(sem);//给信号量的资源加一
        }
    }
}
void find_complete_cb(int rc, const struct Stat *stat,const void *data){

}
ZkClient::ZkClient():m_zhandle(nullptr)
{
}

ZkClient::~ZkClient()
{
    if(m_zhandle!=nullptr){
        //关闭句柄，释放掉资源
        zookeeper_close(m_zhandle);
    }
}
//连接zkserver
void ZkClient::Start()
{
    std::string zkhost = MprpcApplication::GetInstance().GetConfig().Load("zookeeperip");
    std::string zkport = MprpcApplication::GetInstance().GetConfig().Load("zookeeperport");
    std::string connstr = zkhost + ":" + zkport;

    m_zhandle = zookeeper_init(connstr.c_str(),global_watcher,30000,nullptr,nullptr,0);
    if(m_zhandle==nullptr){
        std::cout<<"zookeeperinit error!"<<std::endl;
        exit(EXIT_FAILURE);
    }
    sem_t sem;
    sem_init(&sem,0,0);
    zoo_set_context(m_zhandle,&sem);//设置句柄上下文,给指定的句柄设置信号量

    //等回调函数被调用，也就是成功连接到zkserver，sem才会增加1
    sem_wait(&sem);
    std::cout<<"zookeeper_init success"<<std::endl;
}

void ZkClient::Create(const char *path, const char *data, int datalen, int state)
{
    char path_buffer[128];
    int bufferlen = sizeof(path_buffer);
    int flag;
    //判断path表示的节点是否存在，如果存在，就不在重复创建
    flag = zoo_exists(m_zhandle,path,0,nullptr);
    if(ZNONODE==flag){
        //创建path指定的节点
        flag = zoo_create(m_zhandle,path,data,datalen,&ZOO_OPEN_ACL_UNSAFE,state,path_buffer,bufferlen);
        if(flag!=ZOK){
            std::cout<<"flag:"<<flag<<std::endl;
            std::cout<<"znode create node error.. path:"<<path<<std::endl;
            exit(EXIT_FAILURE);
        }
        std::cout<<"znode create success.. path:"<<path<<std::endl;
    }

}

std::string ZkClient::GetData(const char *path)
{
    char buffer[64];
    int bufferlen = sizeof(buffer);
    int flag = zoo_get(m_zhandle,path,0,buffer,&bufferlen,nullptr);
    if(flag!=ZOK){
        std::cout<<"get znode error ...path:"<<path<<std::endl;
        return "";    
    }

    return buffer;
}
