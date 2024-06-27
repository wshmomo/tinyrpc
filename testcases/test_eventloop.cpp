
#include<pthread.h>
#include<sys/socket.h>
#include <sys/types.h>          
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include<memory>
#include "../include/common/config.h"
#include "../include/common/log.h"
#include "../include/net/fd_event.h"
#include "../include/net/eventloop.h"
#include "../include/net/timer_event.h"
#include "../include/net/io_thread.h"
#include "../include/net/io_thread_group.h"

void test_io_thread(){
    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    // DEBUGLOG("second");
    DEBUGLOG("listenfd=%d",listenfd);

    int opt = 1;
    if((setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(void*)&opt,sizeof(opt))) == -1){
        ERRORLOG("setsockopt = -1");

    }

    if(listenfd == -1){
        ERRORLOG("listenfd = -1");
    }

    sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(8000);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((bind(listenfd, (struct sockaddr *) &my_addr,sizeof(my_addr))) == -1){
        ERRORLOG("create bind error, error[%d]",errno);

    }
            
   

    if((listen(listenfd,10)) == -1){
        ERRORLOG("create listen error, error[%d]",errno);

    }

    rocket::FDEvent *event = new rocket::FDEvent(listenfd);
    event->listen(rocket::FDEvent::IN_EVENT,[listenfd](){
        sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(sockaddr_in);
        int clientfd = accept(listenfd,(sockaddr*)&client_addr,&client_addr_len);
        char* addr_str;
     
        DEBUGLOG("sucess get client [%s:%d]",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

    });

    int i = 0;
    // DEBUGLOG("forth");
    // eventloop->addEpollEvent(event);  //在Eventloop的构造函数中已经初始化了两个，也就是说运行了addEpollEvent（）两次，一次wakeup,一次timer,这两都继承FDEvent
    // DEBUGLOG("five");  //注意：虽然上面已经初始化了timer，他也占据了一个fd，但是有个问题是他占据的这个fd，会创建几次TimerEvent???先打印five,在success create timer event, will excute at [1718113656535]
    //因为success create timer event, will excute at [%lld]在TimerEvent里面，你没创建一次TimerEvent，都会打印一次这个
    rocket::TimerEvent::s_ptr timer_event = make_shared<rocket::TimerEvent>(  //这里这个的作用又是什么？
        2000,true,[&i](){   //这里的意思是每隔2000ms他都会触发一次,这里有个问题为什么他会运行35次就停止触发,这里的计时器有什么作用
            INFOLOG("trigger timer event, count = %d", i++);
        }
    );  //这块啥意思，这块i可以增加是因为后面删掉之后又加上了，也就是说，这个定时器循环使用了，但是为什么触发35次停止


    // rocket::IOThread io_thread;
    // io_thread.getEventLoop()->addEpollEvent(event);
    // io_thread.getEventLoop()->addTimerEvent(timer_event);
    // io_thread.start();
    // io_thread.join();  //这里必修要等待子线程运行完，如果不调用的话，有可能在唤醒loop之前，就直接就运行主线程，那么就会进行析构（也就是说运行到io_thread.cc中的Main的DEBUGLOG("IOThread %d start loop", thread->m_thread_id);之后就直接运行主函数的析构函数，event_loop事件清空，然后在运行子线程，loop就不会进行，直接打印end子线程


   rocket::IOThreadGroup io_thread_group(2);
   io_thread_group.getIOThread()->getEventLoop()->addEpollEvent(event);
   io_thread_group.getIOThread()->getEventLoop()->addTimerEvent(timer_event);

   rocket::IOThread* io_thread2 = io_thread_group.getIOThread();

   io_thread2->getEventLoop()->addTimerEvent(timer_event);

   io_thread_group.start();
   io_thread_group.join();
}

int main(){

    rocket::Config::SetGlobalConfig("/home/wsh/code_rpc/tinyrpc/conf/tinyrpc.xml");
    rocket::Logger::InitGlobalLogger();
    test_io_thread();
    // DEBUGLOG("mmt");

    // rocket::Eventloop* eventloop = new rocket::Eventloop(); //这里进去就开始在构造函数中初始化了，首先初始化wakeup_fd，这里等于4，然后初始化timer_event_fd,这里等于5，接着是可读事件listenfd
    // // DEBUGLOG("first");

    // int listenfd = socket(AF_INET,SOCK_STREAM,0);
    // // DEBUGLOG("second");
    // DEBUGLOG("listenfd=%d",listenfd);

    // int opt = 1;
    // if((setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(void*)&opt,sizeof(opt))) == -1){
    //     ERRORLOG("setsockopt = -1");

    // }

    // if(listenfd == -1){
    //     ERRORLOG("listenfd = -1");
    // }

    // sockaddr_in my_addr;
    // memset(&my_addr, 0, sizeof(my_addr));
    // my_addr.sin_family = AF_INET;
    // my_addr.sin_port = htons(8000);
    // my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // if ((bind(listenfd, (struct sockaddr *) &my_addr,sizeof(my_addr))) == -1){
    //     ERRORLOG("create bind error, error[%d]",errno);

    // }
            
   

    // if((listen(listenfd,10)) == -1){
    //     ERRORLOG("create listen error, error[%d]",errno);

    // }

    // rocket::FDEvent *event = new rocket::FDEvent(listenfd);
    // event->listen(rocket::FDEvent::IN_EVENT,[listenfd](){
    //     sockaddr_in client_addr;
    //     socklen_t client_addr_len = sizeof(sockaddr_in);
    //     int clientfd = accept(listenfd,(sockaddr*)&client_addr,&client_addr_len);
    //     char* addr_str;
     
    //     DEBUGLOG("sucess get client [%s:%d]",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

    // });

    // int i = 0;
    // // DEBUGLOG("forth");
    // eventloop->addEpollEvent(event);  //在Eventloop的构造函数中已经初始化了两个，也就是说运行了addEpollEvent（）两次，一次wakeup,一次timer,这两都继承FDEvent
    // // DEBUGLOG("five");  //注意：虽然上面已经初始化了timer，他也占据了一个fd，但是有个问题是他占据的这个fd，会创建几次TimerEvent???先打印five,在success create timer event, will excute at [1718113656535]
    // //因为success create timer event, will excute at [%lld]在TimerEvent里面，你没创建一次TimerEvent，都会打印一次这个
    // rocket::TimerEvent::s_ptr timer_event = make_shared<rocket::TimerEvent>(  //这里这个的作用又是什么？
    //     2000,true,[&i](){   //这里的意思是每隔2000ms他都会触发一次,这里有个问题为什么他会运行35次就停止触发,这里的计时器有什么作用
    //         INFOLOG("trigger timer event, count = %d", i++);
    //     }
    // );  //这块啥意思，这块i可以增加是因为后面删掉之后又加上了，也就是说，这个定时器循环使用了，但是为什么触发35次停止



    // eventloop->addTimerEvent(timer_event);
    // eventloop->loop();

    




    
    // int i =1;
    // while(i--){
    //     DEBUGLOG("test debug log %s","11");
    //     INFOLOG("test info log %s", "11");

    // }
    return 0;
}