// #pragma once
#ifndef LOG_H
#define LOG_H

#include<iostream>
#include<string>
#include<memory>
#include<queue>
#include<mutex>
#include "util.h"
#include "config.h"




using namespace std;
//开始的日志打印会出现内存泄露的情况，这里进行了修改，不用
//rocket::Logger::GetGlobalLogger()->pushLog((new rocket::LogEvent(rocket::LogLevel::Debug))->toString()+ "[" + string(__FILE__) + ":" + to_string(__LINE__) + "]\t"+rocket::formatString(str, ##__VA_ARGS__)+"\n"); 

#define DEBUGLOG(str,...)\
    if(rocket::Logger::GetGlobalLogger()->getLogLevel() <= rocket::Debug){\
    rocket::Logger::GetGlobalLogger()->pushLog(rocket::LogEvent(rocket::LogLevel::Debug).toString()+ "[" + string(__FILE__) + ":" + to_string(__LINE__) + "]\t"+rocket::formatString(str, ##__VA_ARGS__)+"\n"); \
    rocket::Logger::GetGlobalLogger()->log(); \
    }\




    
#define INFOLOG(str,...)\
    if(rocket::Logger::GetGlobalLogger()->getLogLevel() <= rocket::Info){\
    rocket::Logger::GetGlobalLogger()->pushLog(rocket::LogEvent(rocket::LogLevel::Info).toString()+ "[" + string(__FILE__) + ":" + to_string(__LINE__) + "]\t"+rocket::formatString(str, ##__VA_ARGS__)+"\n"); \
    rocket::Logger::GetGlobalLogger()->log(); \
    }\

#define ERRORLOG(str,...)\
    if(rocket::Logger::GetGlobalLogger()->getLogLevel() <= rocket::Error){\
    rocket::Logger::GetGlobalLogger()->pushLog(rocket::LogEvent(rocket::LogLevel::Error).toString()+ "[" + string(__FILE__) + ":" + to_string(__LINE__) + "]\t"+rocket::formatString(str, ##__VA_ARGS__)+"\n"); \
    rocket::Logger::GetGlobalLogger()->log(); \
    }\


namespace rocket{


template<typename... Args>
string formatString(const char* str, Args&&... args){

    int size = snprintf(nullptr, 0, str, args...);
    string result;
    if(size > 0){
        result.resize(size);
        snprintf(&result[0], size+1, str, args...);  //将str里面的内容写到result中
    }
    return result;
}






enum LogLevel{
    Unkown = 0,
    Debug = 1,
    Info = 2,
    Error = 3
};

string LogLevelToString(LogLevel level);
LogLevel StringToLogLevel(const string& log_level);


class Logger{
public:
    // void log(LogEvent event);

    typedef shared_ptr<Logger> s_ptr;

    Logger(LogLevel level) : m_set_level(level){};

    void pushLog(const string& msg);

    static Logger* GetGlobalLogger();

    static void InitGlobalLogger();

    void log();

    LogLevel getLogLevel() const {
        return m_set_level;
    }

private:

    LogLevel m_set_level;
    queue<string> m_buffer; 
    mutex m_mutex_log;

};


class LogEvent {
public:

    LogEvent(LogLevel level):m_level(level){};

    
    string getFileName() const{
        return m_file_name;
    }

    LogLevel getLogLevel() const{
        return m_level;
    }

    string toString();    //定义互斥锁，保证线程安全
   

private:
    string m_file_name;  //文件名
    string m_file_line;  //行号
    int32_t m_pid;           //进程号，这里用int32_t为了保证移植之后仍然是这个范围
    int32_t m_thread_id;     //线程号
    // string m_time;       //时间
    LogLevel m_level;    //日志级别



    // Logger::s_ptr m_logger;

};






};

#endif