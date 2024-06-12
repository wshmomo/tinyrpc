#include<string.h>
#include "fd_event.h"
#include "log.h"

namespace rocket{
    FDEvent::FDEvent(){
        memset(&m_listen_events,0,sizeof(m_listen_events));
    }

    FDEvent::FDEvent(int fd){
        m_fd = fd;
        memset(&m_listen_events,0,sizeof(m_listen_events));
    }

    FDEvent::~FDEvent(){

    }

    function<void()> FDEvent::handler(TriggerEvent event){
        if(event == TriggerEvent::IN_EVENT){
            return m_read_callback;
        }else{
            return m_write_callback;
        }
    }

    void FDEvent::listen(TriggerEvent event_type, std::function<void()> callback){
        if(event_type == TriggerEvent::IN_EVENT){
            m_listen_events.events |= EPOLLIN;   //这里的m_listen_events.events如果是0的话，那么m_listen_events.envents等于 EPOLLIN,这里用|=是不是很奇怪,为什么不是=
            m_read_callback = callback;
            
        }else{
            m_listen_events.events |= EPOLLOUT;
            m_write_callback = callback;
        }
        m_listen_events.data.ptr = this;
    }

    


}