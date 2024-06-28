#pragma once
#ifndef FDEVENT_H
#define FEEVENT_H

#include<functional>
#include<sys/epoll.h>

namespace rocket{
    class FDEvent{
    public:

        enum TriggerEvent{
            IN_EVENT = EPOLLIN, // 读事件
            OUT_EVENT = EPOLLOUT,  //写事件
            ERROR_EVENT = EPOLLERR, //错误事件
        };

        FDEvent();

        FDEvent(int fd);

        ~FDEvent();

        std::function<void()> handler(TriggerEvent event);

        void listen(TriggerEvent event_type, std::function<void()> callback, std::function<void()> error_callback = nullptr);

        void cancle(TriggerEvent event_type);

        int getFd() const{
            return m_fd;
        }
        
        epoll_event getEpollEvent() {
            return m_listen_events;
        }

        void setNonBlock();

        void setErrorCallback(std::function<void()> cb);
    
    protected:
        int m_fd {-1};

        epoll_event m_listen_events;
        std::function<void()> m_read_callback {nullptr};
        std::function<void()> m_write_callback {nullptr};
        std::function<void()> m_error_callback {nullptr};



    };
}


#endif