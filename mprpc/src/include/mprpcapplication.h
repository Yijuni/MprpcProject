#pragma once

#include "mprpcconfig.h"
#include "mprpcchannel.h"
#include "mprpccontroller.h"

/**
 * mprpc框架初始化类，负责框架的一些初始化操作
 * 比如加载配置文件，提供配置的ip+port
 **/
class MprpcApplication{
public:
    static void Init(int argc,char **argv);
    static MprpcApplication &GetInstance();
    static MprpcConfig& GetConfig();
private:
    static MprpcConfig m_config;   
    MprpcApplication();
    MprpcApplication(const MprpcApplication&)=delete;
    MprpcApplication& operator=(const MprpcApplication&)=delete;
    MprpcApplication(MprpcApplication &&)=delete;
};