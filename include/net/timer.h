#pragma once
#ifndef TIMER_H
#define TIMER_H

#include <map>
#include<mutex>
#include"fd_event.h"
#include "timer_event.h"


namespace rocket{
    class Timer : public FDEvent{
    public:

        Timer();

        ~Timer();

        void addTimerEvent(TimerEvent::s_ptr event);
        void deleteTimerEvent(TimerEvent::s_ptr event);
        void resetArriveTime();
        void onTimer();   //当发生了IO事件，eventlopp会执行这个回调函数，所以只需要将onTimer绑定到readcallback上就行了

    private:
        std::multimap<int64_t, TimerEvent::s_ptr> m_pending_events;
        std::mutex m_mutex_timer;
    };
}

#endif