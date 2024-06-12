#pragma once
#ifndef IOTHREAD_H
#define IOTHREAD_H

#include<pthread.h>
#include<semaphore.h>
#include "eventloop.h"


namespace rocket{
    class IOThread{
    public:
        IOThread();

        ~IOThread();

        static void* Main(void* arg);

        Eventloop* getEventLoop();
        void start();
        void  join();
    
    private:
        pid_t m_thread_id {-1};  // 线程号
        pthread_t m_thread = pthread_self();;  //线程句柄
        Eventloop* m_event_loop {NULL};
        sem_t m_init_semaphore;  //这里的信号量是为了完成线程过程中的同步操作
        sem_t m_start_semaphore;

    };
}


#endif