
#include<sys/time.h>
#include<sstream>
#include<stdio.h>
#include "../../include/common/log.h"

namespace rocket{


    static Logger* g_logger = nullptr;


    Logger* Logger::GetGlobalLogger(){
        
        return g_logger;
        


    }



    void Logger::InitGlobalLogger(){
        
        LogLevel global_log_level = StringToLogLevel(Config::GetGlobalConfig()->m_log_level);
        printf("Init log level [%s]\n", LogLevelToString(global_log_level).c_str());
        g_logger = new Logger(global_log_level);

    }

    string LogLevelToString(LogLevel level){
        switch(level){
            case Debug:
                return "DEBUG";
            case Info:
                return "Info";
            case Error:
                return "Error";
            default:
                return "UNKOWN";
        }
    }




    LogLevel StringToLogLevel(const string& log_level){
        //这里不能用switch，因为跟整形或者枚举类型
        if(log_level == "DEBUG"){
            return Debug;
        }else if(log_level == "INFO"){
            return Info;
        }else if(log_level ==  "ERROR"){
            return Error;
        }else{
            return Unkown;
        }
            

    }

    string LogEvent::toString(){
        struct timeval now_time;   //s和ms
        gettimeofday(&now_time,nullptr);  //得到当前天的时间
        struct tm now_time_t;
        localtime_r(&(now_time.tv_sec),&now_time_t);
        char buf[128];
        strftime(&buf[0],128,"%y-%m-%d %H:%M:%S", &now_time_t);
        string time_str(buf);
        int ms = now_time.tv_usec / 1000;
        time_str = time_str + "." + to_string(ms);

        m_pid = getPid();
        m_thread_id = getThreadId();
        stringstream ss;
        ss << "[" << LogLevelToString(m_level) << "]\t" 
           << "[" << time_str <<"]\t" 
           << "[" << m_pid << ":" << m_thread_id << "]\t";
           

        
        return ss.str();



    }


    void Logger::pushLog(const string& msg){
        lock_guard<mutex> lock(m_mutex_log);
        m_buffer.push(msg);   //感觉需要加锁

    }

    void Logger::log(){
        queue<string> tmp;
        {
            lock_guard<mutex> lock(m_mutex_log);  //这里需要保证锁的最小粒度，所以用了一个中间变量
            m_buffer.swap(tmp);

        }
        while(!tmp.empty()){
            string msg = tmp.front();  //这里需要先进先出   感觉也需要加锁
            tmp.pop();
            printf("%s\n",msg.c_str());
        }
        
    }


}