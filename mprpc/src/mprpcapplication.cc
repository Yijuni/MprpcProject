#include "mprpcapplication.h"
#include <iostream>
#include <unistd.h>
#include <string>

MprpcConfig MprpcApplication::m_config;

void ShowArgsHelp(){
    std::cout<<"format: command -i <configfile>"<<std::endl;
}

void MprpcApplication::Init(int argc, char **argv)
{
    if(argc<2){
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }
    int c = 0;
    std::string config_file;
    while((c=getopt(argc,argv,"i:"))!=-1){//i代表一个命令行选项 :紧跟在i后面代表，i需要一个参数
        switch (c)
        {
        case 'i'://optarg是一个全局变量，用于存储最后一个被解析选项的参数。
            config_file = optarg;
            break;
        case '?'://遇到了无效选项
            ShowArgsHelp();
            exit(EXIT_FAILURE);
        case ':'://表示一个需要参数的选项给有给出参数
            ShowArgsHelp();
            exit(EXIT_FAILURE);
        default:
            break;
        }
    }
    //开始加载配置文件 rpcserver_ip rpcserver_port zookeeper_ip_ zookeeper_port

    m_config.LoadConfigFile(config_file.c_str());
    std::cout<<"rpcserverip:"<<m_config.Load("rpcserverip")<<std::endl;
    std::cout<<"rpcserverport:"<<m_config.Load("rpcserverport")<<std::endl;
    std::cout<<"zookeeperip:"<<m_config.Load("zookeeperip")<<std::endl;
    std::cout<<"zookeeperport:"<<m_config.Load("zookeeperport")<<std::endl;
}

MprpcApplication &MprpcApplication::GetInstance()
{
    static MprpcApplication ins;
    return ins;
}

MprpcApplication::MprpcApplication()
{
}
