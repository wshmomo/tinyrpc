#include<sys/socket.h>
#include<sys/epoll.h>
#include<sys/eventfd.h>
#include "eventloop.h"
#include "log.h"
#include "util.h"

    /*下面的ADD_TO_EPOLL里面必须这样写
    epoll_event tmp = event->getEpollEvent(); \
    int rt = epoll_ctl(m_epoll_fd,op,event->getFd(), &tmp);\
    这样写是错误的
    int rt = epoll_ctl(m_epoll_fd,op,event->getFd(), &(event->getEpollEvent()));\
    */


namespace rocket{
    #define ADD_TO_EPOLL()\
    auto it = m_listen_fds.find(event->getFd()); \
    int op = EPOLL_CTL_ADD;      \
    if(it != m_listen_fds.end()) \
    {   \
        op = EPOLL_CTL_MOD;    \
    } \
    epoll_event tmp = event->getEpollEvent(); \
    int rt = epoll_ctl(m_epoll_fd,op,event->getFd(), &tmp);\
    if(rt == -1){ \
        ERRORLOG("failed epoll_ctl when add fd %d, epoll_ctl error, error info[%d]", event->getFd(),errno);\
        exit(0);\
    }\
    DEBUGLOG("add event success, fd[%d]",event->getFd());\

#define Delete_To_Epoll()\
    auto it = m_listen_fds.find(event->getFd());\
    if(it == m_listen_fds.end()){   \
        return;   \
    }\
    epoll_event tmp = event->getEpollEvent(); \
    int rt = epoll_ctl(m_epoll_fd,EPOLL_CTL_DEL,event->getFd(), &tmp);\
    if(rt == -1){  \
        ERRORLOG("failed epoll_ctl when add fd %d, epoll_ctl error, error info[%d]", event->getFd(),errno);\
        exit(0);\
    }\
    DEBUGLOG("add event success, fd[%d]",event->getFd());\
    


    //需要判断当前线程是否有 Eventloop，如果有的话需要报错
    static thread_local Eventloop* t_current_eventlop =  NULL;          //线程局部变量，这里意思是每个线程都有一个Eventloop实例，他们之间互不影响，用于多线程中，无需显式使用线程同步机制（例如互斥锁）来保护共享数据
    

    static int g_epoll_max_timeout = 10000;  //超时时间
    static int g_epoll_max_events = 10;  //最大的监听事件



    Eventloop::Eventloop(){
        if(t_current_eventlop != NULL){
            ERRORLOG("failed to create event loop, this thread has create event loop");
            exit(0);
        }
        m_thread_id = getThreadId();
        
        //创建成功就返回根节点的fd，如果失败就返回-1
        m_epoll_fd = epoll_create(10);   //这里随便给参数，因为从Linux2.6之后，这个参数就没什么用了，比0大就可以了

        if(m_epoll_fd == -1){
            ERRORLOG("failed to create event loop, epoll_create error, error info[%d]", errno);
            exit(0);
        }

       
        initWakeUpFdEvent();
        initTimer();
        // epoll_event event;
        // event.events = EPOLLIN;  //监听读事件
        // int rt = epoll_ctl(m_epoll_fd,EPOLL_CTL_ADD,m_wakeup_fd,&event);
        // if(rt == -1){
        //     ERRORLOG("failed to create event loop, epoll_ctl epoll_ctl error, error info[%d]", errno);
        //     exit(0);

        // }

        INFOLOG("success create event loop in thread %d",m_thread_id);
        t_current_eventlop = this;

        


    }


    //正常来说这个析构函数也是调不到的，因为这里的服务是死循环，这个对象是不会析构的
    Eventloop::~Eventloop(){
        close(m_epoll_fd);
        if(m_wakeup_fd_event != NULL){
            delete m_wakeup_fd_event;
            m_wakeup_fd_event = NULL;
        }

        if(m_timer != NULL){
            delete m_timer;
            m_timer = NULL;
        }

    }


    void Eventloop::initWakeUpFdEvent(){
        
        m_wakeup_fd = eventfd(0,EFD_NONBLOCK); //非阻塞
        DEBUGLOG("mmt_initwakeupfd %d",m_wakeup_fd)
        if(m_wakeup_fd  < 0){
            ERRORLOG("failed to create event loop, eventfd vreate error, error info[%d]", errno);
            exit(0);

        }

        m_wakeup_fd_event = new WakeUpFdEvent(m_wakeup_fd);

        m_wakeup_fd_event->listen(FDEvent::IN_EVENT,[this](){
            char buf[8];
            //这里需要一直读
            //EAGIN表示：某些文件描述符（如套接字）的缓冲区中没有足够的数据可供读取，或者该描述符不支持非阻塞 I/O，导致调用程序需要等待更多数据到达。
            while((read(m_wakeup_fd,buf,8)) != -1 && errno != EAGAIN) {

            }
            //感觉这里写的有问题，如果一直读，但是读出来的结果返回-1说明你没读到发生了错误呀
            DEBUGLOG("read full bytes from wakeup fd[%d]", m_wakeup_fd);

        }); //读事件

        addEpollEvent(m_wakeup_fd_event);
        
    }



    void Eventloop::initTimer(){
        m_timer = new Timer();
        addEpollEvent(m_timer);

    }

    void Eventloop::addTimerEvent(TimerEvent::s_ptr event){
        m_timer->addTimerEvent(event);
    }

    int t = 0;
    void Eventloop::loop(){
        

        while(!m_stop_flag){
            DEBUGLOG("times of loop %d",t++);
            queue<function<void()>> tmp_tasks;
            {
                lock_guard<mutex> lock(m_mutex);
                // tmp_tasks = m_pending_tasks;  //这里有问题，它怎么可能等于在交换呢
                m_pending_tasks.swap(tmp_tasks);  //交换两个队列里面的内容
            }

            while(!tmp_tasks.empty()){
                std::function<void()> cb = tmp_tasks.front();    //这个是去执行队列里面的任务
                tmp_tasks.pop();        //执行完了所以需要删掉
                if(cb){
                    cb();
                }
                 
                 //原来的写法
                // tmp_tasks.front()();    //这个是去执行队列里面的任务
                // tmp_tasks.pop();        //执行完了所以需要删掉
            }
            

            //如果有定时任务需要执行，那么执行
            //1.如何判断一个定时任务需要执行？（now() > TimerEvent.arrtive_time)
            //2. arrtive_time如何让eventloop监听
            int timeout = g_epoll_max_timeout;
            epoll_event result_events[g_epoll_max_events];

            int rt = epoll_wait(m_epoll_fd, result_events,g_epoll_max_events,timeout);  //有一个问题，现在这里的m_epoll_fd已经添加了三个可读事件了为什么只得到了两
            DEBUGLOG("now end epoll_wait, rt = %d",rt);

            if(rt < 0){
                ERRORLOG("epoll_wait error, error = %d", errno);
            }else{
                for(int i = 0; i < rt; i++){
                    epoll_event trigger_event = result_events[i];
                    FDEvent* fd_event = static_cast<FDEvent*> (trigger_event.data.ptr);
                    if(fd_event == NULL){
                        continue;
                    }
                    if(trigger_event.events & EPOLLIN){  //这里啥意思两任何一个等于1都可以，我怎么感觉这块应该是&
                        addTask(fd_event->handler(FDEvent::IN_EVENT));

                    }
                    if(trigger_event.events & EPOLLOUT){  //这里啥意思两任何一个等于1都可以，我怎么感觉这块应该是&
                        addTask(fd_event->handler(FDEvent::OUT_EVENT));

                    }
                
                }

            }

 
            

        }

    }

    void Eventloop::wakeup(){
        m_wakeup_fd_event->wakeup();
        

    }

    void Eventloop::stop(){
        m_stop_flag = true;

    }

    void Eventloop::dealWakeup(){

    }
    
    //这样写是为了保证线程安全问题，主从Rector里面，
    //比如假设isInloopThread()等于true表示主线程，在主线程中调用了子线程，子线程IO方法，由于跨线程这里isInloopThread()等于false,但是这里并没有添加成功，
    //因为你返回了一个cb函数，这里把它添加到了m_pending_tasks中，只有等loop()函数中的运行他才会运行
    void Eventloop::addEpollEvent(FDEvent* event){
        if(isInloopThread()){  //如果是这个线程进行下面的
            ADD_TO_EPOLL();

        }else{ //如果不是这个线程
                auto cb = [this,event](){
                    // auto it = m_listen_fds.find(event->getFd());
                    // int op = EPOLL_CTL_ADD;   //没找到添加
                    // if(it != m_listen_fds.end()){   //找到了，找到了为什么要修改
                    //     op = EPOLL_CTL_MOD;    
                    // }
                    // int rt = epoll_ctl(m_epoll_fd,op,event->getFd(), &(event->getEpollEvent()));

                    // if(rt == -1){  //这里它写错的
                    //     ERRORLOG("failed epoll_ctl when add fd %d, epoll_ctl error, error info[%d]", event->getFd(),errno);
                    //     exit(0);

                    // }
                    ADD_TO_EPOLL();

                };

                addTask(cb,true);


        }

    }


    void Eventloop::deleteEpollEvent(FDEvent* event){
        if(isInloopThread()){  
            Delete_To_Epoll();

        }else{ 
            auto cb = [this,event](){
                Delete_To_Epoll();
            };
            addTask(cb,true);

        }
        
    }

    bool Eventloop::isInloopThread(){
        return getThreadId() == m_thread_id ;

    }

    void Eventloop::addTask(function<void()> cb, bool is_wake_up /*=false*/){
        {
            lock_guard<mutex> lock(m_mutex);
            m_pending_tasks.push(cb);   //这里回调函数搞进去的是加入事件，加入监听事件和删除监听事件
        }

        if(is_wake_up){
            wakeup();
        }

        

    }


}
