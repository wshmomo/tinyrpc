#include "../../include/net/timer_event.h"
#include "../../include/net/fd_event.h"
#include "../../include/common/log.h"


namespace rocket{
    TimerEvent::TimerEvent(int interval, bool is_repeated, std::function<void()> cb)
         : m_interval(interval), m_is_repeated(is_repeated),m_task(cb){
            
        resetArriveTime();

        

    }

    void TimerEvent::resetArriveTime(){
        m_arrive_time = getNowMs() + m_interval;    //下一次执行的事件等于当前事件加上时间间隔
        DEBUGLOG("success create timer event, will excute at [%lld]",m_arrive_time);

     }
}