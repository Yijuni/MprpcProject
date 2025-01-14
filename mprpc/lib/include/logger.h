#pragma once
#include <lockqueue.h>
#include <string>
//mprpc框架日志系统

enum LogLevel{
    INFO,//普通的日志信息
    ERROR,//错误信息
};

class Logger{
public:
    //获取日志的单例
    static Logger& GetInstance();
    //设置日志级别
    void SetLogLevel(LogLevel level);
    //设置写日志
    void Log(std::string msg);
private:
    LogLevel m_loglevel;//日志级别
    LockQueue<std::string> m_lockqueue;//日志缓冲队列
    Logger();
    Logger(const Logger&)=delete;
    Logger& operator=(const Logger&)=delete;
};

//定义打印日志的宏
#define LOG_INFO(format,...)\
    do{\
        char buf[1024]={0};\
        Logger &logger = Logger::GetInstance();\
        logger.SetLogLevel(LogLevel::INFO);\
        snprintf(buf,1024,format,##__VA_ARGS__);\
        logger.Log(buf);\
    }while(0);

#define LOG_ERROR(format,...) \
    do{\
        char buf[1024]={0};\
        Logger &logger = Logger::GetInstance();\
        logger.SetLogLevel(ERROR);\
        snprintf(buf,1024,format,##__VA_ARGS__);\
        logger.Log(buf);\
    }while(0);