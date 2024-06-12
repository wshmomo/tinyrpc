#pragma once
#ifndef WAKEUPFDEVENT_H
#define WAKEUPFDEVENT_H

#include "fd_event.h"
namespace rocket{
    class WakeUpFdEvent : public FDEvent{

    public:
        WakeUpFdEvent(int fd);
        ~WakeUpFdEvent();

        //触发读事件
        void wakeup();  

    private:

    };
}


#endif