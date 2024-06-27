#pragma once
#ifndef TCPACCPTOR_H
#define TCPACCPTOR_H

#include<memory>

#include "../common/log.h"
#include "net_addr.h"

namespace rocket{
    
    class TcpAcceptor{
    public:



        typedef shared_ptr<TcpAcceptor> s_ptr; 
        TcpAcceptor(NetAddr::s_ptr local_addr);

        ~TcpAcceptor();
         
        //为什么要封装呢，因为还要适用于其他协议
        pair<int, NetAddr::s_ptr> accept();

        int getListenfd(){
            DEBUGLOG("socket listenfd %d",m_listenfd);
            return m_listenfd;
        }

    private:
        //服务端监听的地址：ip:port
        //listenfd
        NetAddr::s_ptr m_local_addr;  //为什么这里是NetAddr而不是IPNetAddr,为了适用于不同的网络，不仅仅是IP的

        int m_listenfd {-1}; // 监听套接字

        int m_family {-1};


    };
}


#endif