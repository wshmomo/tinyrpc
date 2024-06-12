#include<sys/timerfd.h>
#include<string.h>
#include "timer.h"
#include "log.h"
#include "util.h"

namespace rocket{

    Timer::Timer() : FDEvent(){

        m_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
        DEBUGLOG("timer fd = %d",m_fd);
        
        //把fd的可读事件放到了eventloop中监听，这里你从哪儿看出来放到了event_ctl上了，明明还没有放
        listen(FDEvent::IN_EVENT,bind(&Timer::onTimer,this));  //注意每个类的成员函数都有一个默认参数this，并且这里和FDEvent属于继承关系，FDEvent中有listen()函数，所以Timer应该也有

    }

    Timer::~Timer(){
        

    }

    void Timer::resetArriveTime(){
        std::multimap<int64_t, TimerEvent::s_ptr> tmp;

        {
            lock_guard<mutex> lock(m_mutex_timer);
            tmp = m_pending_events;
        }

        if(tmp.size() == 0){
            return;
        }


        int64_t now = getNowMs();  //获取当前时间之后
        auto it = tmp.begin();  
        int64_t interval = 0; 
        if(it->second->getArriveTime() > now){        //it->second->getArriveTime()是任务发生时间加上时间间隔,这里说明第一个任务的定时事件还没到
            interval = it->second->getArriveTime() - now;  //这里的间隔就是定时任务的时间？这里不减now嘛？

        }else{
            interval = 100; //说明定时任务已经过期，说明这个任务的时间已经到了，但是我们还没有执行，这里需要拯救


        }

        timespec ts;
        memset(&ts,0,sizeof(ts));  //这里和下面那个必须初始化，要不然会使一些调用出现非法的值导致timerfd_settime出现问题
        ts.tv_sec = interval / 1000;  //他的秒等于我们的毫秒数除以1000
        ts.tv_nsec = (interval % 1000) * 100000;

        itimerspec value;
        memset(&value,0,sizeof(value));  //这里
        value.it_value = ts;

        
        int rt = timerfd_settime(m_fd,0,&value,NULL);  //m_fd在指定的时间触发可读事件，触发可读事件之后，eventloop就可以监听到
        if(rt != 0){
            ERRORLOG("timerfd_settime error, errno = %d, error = %s", errno,strerror(errno));
        }
        DEBUGLOG("timer reset to %lld", now+interval);
 

    }

    void Timer::addTimerEvent(TimerEvent::s_ptr event){
        bool is_reset_timerfd = false;   //判断是否需要重新设置超时时间，这里默认不需要重新设置
        {
            lock_guard<mutex> lock(m_mutex_timer);
            if(m_pending_events.empty()){    //如果当前的事件是空的话，我们需要设定下一次fd的时间，否则我们判断一下我们添加的定时任务在哪里
               is_reset_timerfd = true;
            }else{
                auto it = m_pending_events.begin();
                if((*it).second->getArriveTime()  > event->getArriveTime()){  
                //如果这个时间比当前我们插入的这个时间大的话，意思就是当前我们需要插入的这个定时任务比我们当前任务队列中所有的任务的定时事件都要早,为了不让当前这个任务延时，所以需要重设定时时间
                    is_reset_timerfd = true;
               
              
                }
            }
            m_pending_events.emplace(event->getArriveTime(),event);   //因为这里是set，所以会根据索引(这里是定时时间)排序(默认从小到大)
        

        }
        if(is_reset_timerfd){
            // DEBUGLOG("is_reset_timerfd = %d",is_reset_timerfd);
            resetArriveTime();

        }


    }
    void Timer::deleteTimerEvent(TimerEvent::s_ptr event){
        event->setCancled(true);
        {
            lock_guard<mutex> lock(m_mutex_timer);

            auto begin = m_pending_events.lower_bound(event->getArriveTime());  //第一个小于event->getArriveTime()
            auto end = m_pending_events.upper_bound(event->getArriveTime());   //第一个大于event->getArriveTime()
            auto it = begin;
            for(it; it != end; ++it){
                if((*it).second == event){
                    break;
                }
            }

            if(it != end){
                m_pending_events.erase(it);
            }

        }

        DEBUGLOG("success delete TimerEvent at arrive time %lld",event->getArriveTime());


    }
    void Timer::onTimer(){
        //触发可读事件之后执行的回调函数
        //因为这里的m_fd可读，所以我们需要把这个数据读掉，要不然下次epoll还会读，注意这里是水平触发
        //处理缓冲区数据，防止下一次触发可读事件？？？等会儿去看看为啥
        DEBUGLOG("ontimer");
        char buf[8];
        while(1){
            if((read(m_fd,buf,8))== -1 && errno ==EAGAIN){
                break;   //说明读完了
            } 
        }

        //执行定时任务
        int64_t now = getNowMs();
        std::vector<TimerEvent::s_ptr> tmps;
        vector<pair<int64_t, std::function<void()>> >tasks;

        {
            lock_guard<mutex> lock(m_mutex_timer);
            auto it = m_pending_events.begin();
            for(it; it != m_pending_events.end(); it++){
                if((*it).first <= now){  // 当定时任务还没有被执行时，并且没有被取消时
                    if(!(*it).second->isCancled()){
                        tmps.push_back((*it).second);
                        tasks.push_back(std::make_pair((*it).second->getArriveTime(),(*it).second->getCallBack()));

                    }
                }else{
                    break;

                }
            }

            m_pending_events.erase(m_pending_events.begin(), m_pending_events.end());

        }
        //这里删掉再重新添加就是让定时器能够一直++的原因
        //为什么上面已经删掉了，但是这里却还要重新添加
        //需要把重复的Event 再次添加 为什么需要再次添加，因为这里的m_pending_events是个map,他实际是根据getArriveTime()排序的，重复添加进去，我们会先去调整arrivetime的，然后，如果不删掉的话，没办法重新排序的
        //如果不重新设置定时时间并重新添加任务，任务可能会丢失或者不会按预期时间再次执行。在很多定时器实现中，任务调度是基于时间排序的。每次任务执行后需要重新添加，以确保它被插入到合适的位置，从而在正确的时间被调度。
        for(auto i = tmps.begin(); i != tmps.end(); i++){
            if((*i)->isRepeated()){
                (*i)->resetArriveTime();
                addTimerEvent(*i);
            }
        }
        resetArriveTime();
        
        //执行定时任务
        for(auto i: tasks){
            if(i.second){
                i.second();
            }
        }

    }
}