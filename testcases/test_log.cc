#include "log.h"
#include<pthread.h>
#include "config.h"
#include<mutex>

void* fun(void*){
    int i = 1;
    while(i--){
        DEBUGLOG("debug this is thread in %s","fun");
        INFOLOG("info this is thread in %s","fun");

    }

    return nullptr;
}

int main(){

    rocket::Config::SetGlobalConfig("/home/wsh/code_rpc/tinyrpc/conf/tinyrpc.xml");
    rocket::Logger::InitGlobalLogger();
    
    pthread_t thread;
    pthread_create(&thread,NULL,&fun,NULL);


    
    int i =1;
    while(i--){
        DEBUGLOG("test debug log %s","11");
        INFOLOG("test info log %s", "11");

    }
    pthread_join(thread,NULL);   //这里需要回收线程
    return 0;
}