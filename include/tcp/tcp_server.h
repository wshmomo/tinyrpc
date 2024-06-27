#pragma once
#ifndef TCPSERVER_H
#define TCPSERVER_H

#include<set>
#include "tcp_acceptor.h"
#include "tcp_connection.h"
#include "net_addr.h"
#include "../net/eventloop.h"
#include "../net/io_thread_group.h"

#include "../net/fd_event.h"


namespace rocket{
    class TcpServer{
    public:
        TcpServer(NetAddr::s_ptr local_addr);
        ~TcpServer();

        void start();

    private:
        //主要为了给监听套接字添加到主线程的eventloop里
        void init();

        //当有客户端来连接时他会执行
        void onAccept();

    private:
        TcpAcceptor::s_ptr m_acceptor;
        NetAddr::s_ptr m_local_addr; // 本地监听地址

        Eventloop* m_main_event_loop {NULL};  // mainReactor
        IOThreadGroup* m_io_thread_group {NULL};  //subReactor组
        FDEvent* m_listen_fd_event;

        int m_client_counts;  //表示当前的新连接个数
        set<TcpConnection::s_ptr> m_client;


    };
}


#endif