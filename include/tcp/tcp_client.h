#pragma once
#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include<memory>
#include "net_addr.h"
#include "../net/eventloop.h"
#include "tcp_connection.h"
#include "../coder/abstract_protocol.h"

namespace rocket{
    class TcpClient{
    public:
        typedef shared_ptr<TcpClient> s_ptr;
        TcpClient(NetAddr::s_ptr peer_addr);

        ~TcpClient();

        //异步的进行connect
        //如果connect成功，done会被执行
        void connect(std::function<void()> done);
        

        //异步的发送Message
        //如果发送Message成功，会调用done函数， 函数的入参就是message对象
        void writeMessage(AbstractProtocol::s_ptr request, std::function<void(AbstractProtocol::s_ptr )> done);

        //异步的读取Message
        //如果读取Message成功，会调用done函数， 函数的入参就是message对象
        void readMessage(const string& msg_id, std::function<void(AbstractProtocol::s_ptr )> done);

        void stop();
    
    private:
        NetAddr::s_ptr m_peer_addr;  //对端的网络地址，代表我们要connect的地址
        Eventloop* m_event_loop {NULL};  //代表我们当前所用的Eventloop,指针最好要初始化

        int m_fd {-1};  //对端fd
        FDEvent* m_fd_event {NULL};  //
        TcpConnection::s_ptr m_connection;  //这里是为了用里面的一些东西完成收发

        int m_connect_errcode {0};
        string m_connect_error_info;

    };
}



#endif