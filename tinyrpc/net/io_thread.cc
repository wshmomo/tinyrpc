#include <assert.h>
#include "log.h"
#include "io_thread.h"
#include "util.h"

namespace rocket{
    IOThread::IOThread(){

        int rt = sem_init(&m_init_semaphore,0,0);//这里的信号量是为了完成线程过程中的同步操作
        int rt1 = sem_init(&m_start_semaphore,0,0);//这里的信号量是为了完成线程过程中的同步操作
        assert(rt == 0);  //断言检查，不等于0需要终止
        assert(rt1 == 0);  //断言检查，不等于0需要终止
        pthread_create(&m_thread,NULL,&IOThread::Main,this); 
        
        //也就是说这里需要做一下同步操作
        // wait,知道新线程执行完Main函数的前置，也就是说运行到thread->m_event_loop->loop()这一步，然后主线程才能往下走
        sem_wait(&m_init_semaphore);  //信号量+1才会返回,返回之后才能往下运行也就是DEBUGLOG

        DEBUGLOG("TOThread [%d] create success", m_thread_id);

    }



    IOThread::~IOThread(){
        m_event_loop->stop();
        sem_destroy(&m_init_semaphore);
        sem_destroy(&m_start_semaphore);
        pthread_join(m_thread,NULL);//等待当前线程结束 //有一个问题如何结束loop循环呢
        if(m_event_loop){
            delete m_event_loop;
            m_event_loop = NULL;
        }

    }

    void* IOThread::Main(void* arg){  //所以这里需要传参，传一个this指针，才会运行这些
        IOThread* thread = (IOThread*) arg;

        thread->m_event_loop = new Eventloop();
        thread->m_thread_id = getThreadId();
        
        //唤醒等待的线程
        sem_post(&thread->m_init_semaphore);

        //在IO线程等待，知道我们主动启动
        DEBUGLOG("IOThread %d created, wait start semaphore",thread->m_thread_id);
        sem_wait(&thread->m_start_semaphore);
        DEBUGLOG("IOThread %d start loop", thread->m_thread_id);
        thread->m_event_loop->loop();

        DEBUGLOG("IOThread %d end loop",thread->m_thread_id);

    }

    Eventloop* IOThread::getEventLoop(){
        return m_event_loop;
        
    }

    void IOThread::start(){
        DEBUGLOG("NOw invoke IOThread %d",m_thread_id);
        sem_post(&m_start_semaphore);

    }


    void  IOThread::join(){
        //主线程一直等,等到子线程去结束
        pthread_join(m_thread,NULL);


    }

}