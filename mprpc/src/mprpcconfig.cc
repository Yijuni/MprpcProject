#include "mprpcconfig.h"
#include <iostream>
#include <string>
void MprpcConfig::LoadConfigFile(const char *config_file)
{
    FILE *pf = fopen(config_file,"r");
    if(pf==nullptr){
        std::cout<<config_file<<"is not exist!!"<<std::endl;
        exit(EXIT_FAILURE);
    }
    //1.注释：2.正确的配置项 3.去掉开头多余的空格
    while(!feof(pf)){//没有读到文件结尾
        char buf[512]={0};
        fgets(buf,512,pf);//读取一行
        std::string read_buf(buf);

        //去掉字符串前后多余的空格
        Trim(read_buf);

        //判断注释
        if(read_buf[0]=='#' || read_buf.empty()){
            continue;//继续读下一行
        }

        //解析配置项
        int idx = read_buf.find('=');
        if(idx==-1){
            //配置项不合法
            //LOG..INFO
            continue;
        }
        std::string key;
        std::string value;
        key = read_buf.substr(0,idx);
        Trim(key);//去掉前后空格
        value = read_buf.substr(idx+1);
        Trim(value);//去掉前后空格
        m_configMap[key] = value;
    }
}

std::string MprpcConfig::Load(const std::string &key)
{
    auto it = m_configMap.find(key);
    if(it==m_configMap.end()){
        return "";
    }
    return it->second;
}

void MprpcConfig::Trim(std::string &src_buf)
{
    int idx = src_buf.find_first_not_of(' ');//找到第一个不是空格字符下标
    if(idx!=std::string::npos){
        src_buf = src_buf.substr(idx);
    }
    //去掉字符串后面多余的空格，先把换行符给去掉
    if(src_buf[src_buf.size()-1] =='\n'){
        src_buf.erase(src_buf.end()-1);
    }
    idx = src_buf.find_last_not_of(' ');
    if(idx!=std::string::npos){
        src_buf = src_buf.substr(0,idx+1);
    }
}
