#include<string.h>
#include<fcntl.h>
#include "../../include/net/fd_event.h"
#include "../../include/common/log.h"

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

    void FDEvent::cancle(TriggerEvent event_type){
        if(event_type == TriggerEvent::IN_EVENT){
            m_listen_events.events &= (~EPOLLIN);   //这里相当于移除其可读事件
            
        }else{
            m_listen_events.events &= (~EPOLLOUT);  //移除其可写事件
        }


    }

    void FDEvent::setNonBlock(){
        int flag = fcntl(m_fd, F_GETFL,0);
        if(flag & O_NONBLOCK){
            return;
        }
        fcntl(m_fd, F_SETFL, flag | O_NONBLOCK);


    }

    


}