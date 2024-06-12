#include<unistd.h>
#include "wakeup_fd_event.h"
#include "log.h"

namespace rocket{

    WakeUpFdEvent::WakeUpFdEvent(int fd) : FDEvent(fd){

    }
    WakeUpFdEvent::~WakeUpFdEvent(){

    }

    void WakeUpFdEvent::wakeup(){
        char buf[8] = {'a'};
        
        int rt = write(m_fd,buf,8);  //从buf中往m_fd中写，如果成功返回实际写入的字节数

        if(rt != 8){
            ERRORLOG("write to wakeup fd less than 8 bytes, fd[%d]", m_fd);

        }


    }

}