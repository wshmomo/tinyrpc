#pragma once
#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

// #include<memory>
#include<vector>
#include<map>
#include "net_addr.h"
#include "tcp_buffer.h"
#include "../net/io_thread.h"
#include "../common/log.h"
#include "../net/fd_event_group.h"
#include "../net/eventloop.h"
#include "../coder/abstract_protocol.h"
#include "../coder/abstract_coder.h"
#include "../rpc/rpc_dispatcher.h"

namespace rocket{



    enum TcpState{
        NotConnected = 1,
        Connected,
        HalfClosing,
        Closed,
    };

    enum TcpConnectionType{
        TcpConnectionByServer = 1, // 作为服务端使用，代表一个客户端连接
        TcpConnectionByClient,  //作为客户端使用，代表跟对端服务器连接

    };

    class TcpConnection{
    public:



        typedef shared_ptr<TcpConnection> s_ptr;
        //输入参数：连接的线程, clientfd(表示他是哪个客户端)， 缓冲区大小， 对端地址
        TcpConnection(Eventloop* eventloop, int fd, int buffer_size, NetAddr::s_ptr peer_addr, NetAddr::s_ptr local_addr, TcpConnectionType type = TcpConnectionByServer); 
        ~TcpConnection();

        void OnRead();

        void excute();

        void OnWrite();

        void setState(const TcpState state);

        TcpState getState();

        void clear();   //关闭连接

        void shutdown();  //服务器主动关闭连接，也就是服务端去触发四次挥手的过程，这里是为了处理无效恶意的TCP连接，

        void setConnectionType(TcpConnectionType type);

        //启动监听可写事件
        void listenWrite();

        //启动监听可读事件
        void listenRead();

        void pushSendMessage(AbstractProtocol::s_ptr,function<void(AbstractProtocol::s_ptr)>);

        void pushReadMessage(const string& req_id,function<void(AbstractProtocol::s_ptr)>);

        NetAddr::s_ptr getLocaladdr();
        NetAddr::s_ptr getPeeraddr();
    
    private:
    
        Eventloop* m_eventloop {NULL};  //代表持有该连接的IO线程
        NetAddr::s_ptr m_local_addr; // 本地地址
        NetAddr::s_ptr m_peer_addr; //对端地址

        TcpBuffer::s_ptr m_in_buffer;  //接收缓冲区
        TcpBuffer::s_ptr m_out_buffer; //发送缓冲区

        

        FDEvent* m_fd_event {NULL};

        TcpState m_state;

        int m_fd {0};

        TcpConnectionType m_connection_type {TcpConnectionByServer};


        //因为这里我们需要把智能指针给寸下来，才能取到他的相关参数,回调函数的记录
        vector<pair<AbstractProtocol::s_ptr,function<void(AbstractProtocol::s_ptr)>>> m_write_dones;


        //key is req_id
        map<string ,function<void(AbstractProtocol::s_ptr)>> m_read_dones;

        AbstractCode* m_coder {NULL};

        // shared_ptr<RpcDispatcher> m_dispatcher;

        


    };
}



#endif