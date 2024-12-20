#pragma once

//mprpc框架初始化类，负责框架的一些初始化操作
class MprpcApplication{
public:
    static void Init(int argc,char **argv);
    static MprpcApplication &GetInstance();
private:
    MprpcApplication();
    MprpcApplication(const MprpcApplication&)=delete;
    MprpcApplication& operator=(const MprpcApplication&)=delete;
    MprpcApplication(MprpcApplication &&)=delete;
};