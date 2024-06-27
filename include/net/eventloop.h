// #pragma once
#ifndef EVENTLOOP_H
#define EVENTLOOP_H


#include<pthread.h>
#include<set>
#include<functional>
#include<queue>
#include<mutex>
#include "fd_event.h"
#include "wakeup_fd_event.h"
#include "timer.h"
#include "../common/log.h"
#include "../common/util.h"

using namespace std;

namespace rocket{
    class Eventloop{
    public: 
        Eventloop();

        ~Eventloop();

        void loop();

        void wakeup();

        void stop();  //正常情况下stop函数不会被调用的，因为服务器一旦被启用就会一直运行下去

        void addEpollEvent(FDEvent* event);


        void deleteEpollEvent(FDEvent* event);

        //添加之前我们需要判断是否还有其它线程，因为可能会发生条件竞争
        bool isInloopThread();

        //添加回调函数到任务中
        void addTask(function<void()> callback, bool is_wake_up = false);

        //添加定时任务的方法
        void addTimerEvent(TimerEvent::s_ptr event);
        

        //获取当前线程的Eventloop对象
        static Eventloop* getCurrentEventloop();

        bool isLooping();





    private:

        void dealWakeup();

        //初始化WakeUpFdEvent
        void initWakeUpFdEvent();

        //初始化Timer
        void initTimer();


        pid_t m_thread_id;  //线程号
        int m_epoll_fd {0};  //epoll句柄

        int m_wakeup_fd {0}; //来唤醒epoll_wait  

        WakeUpFdEvent* m_wakeup_fd_event {NULL};

        bool m_stop_flag {false};  //stop标志
        set<int> m_listen_fds;   //没搞明白这里的这个参数是什么意思

        queue<function<void()>> m_pending_tasks;  //所有待执行的任务队列，这里的任务不传参？？？？

        //定义互斥锁，保证线程安全
        mutex m_mutex;

        Timer* m_timer {NULL};

        bool m_is_looping {false};

    };
}



#endif