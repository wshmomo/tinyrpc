#include "../../include/tcp/tcp_server.h"
#include "../../include/tcp/tcp_connection.h"

namespace rocket{
    //注意这里的TcpServer是全局的单例对象，只能在主线程里面去构建这个对象，不能在IO线程里去构建
    TcpServer::TcpServer(NetAddr::s_ptr local_addr) : m_local_addr(local_addr){


        init();

        INFOLOG("rocket TcpServer lisetn sucess on [%s]", m_local_addr->toString().c_str());

    }

    TcpServer::~TcpServer(){
        if(m_main_event_loop){
            delete m_main_event_loop;
            m_main_event_loop = NULL;
        }
        if(m_io_thread_group){
            delete m_io_thread_group;
            m_io_thread_group = NULL;
        }
        if(m_listen_fd_event){
            delete m_listen_fd_event;
            m_listen_fd_event = NULL;
        }

    }

    void TcpServer::init(){
        m_acceptor = make_shared<TcpAcceptor>(m_local_addr);
        m_main_event_loop = Eventloop::getCurrentEventloop();
        m_io_thread_group = new IOThreadGroup(2);   //先设置一个简单的值，后面设置到配置文件中
        
        m_listen_fd_event = new FDEvent(m_acceptor->getListenfd());  //如果是局部变量的话，这里init运行完就会直接析构掉
        m_listen_fd_event->listen(FDEvent::IN_EVENT, std::bind(&TcpServer::onAccept,this));
        m_main_event_loop->addEpollEvent(m_listen_fd_event);
    }


    void TcpServer::onAccept(){
        auto re = m_acceptor->accept();
        int client_fd = re.first;
        NetAddr::s_ptr peer_addr= re.second;
        // FDEvent client_fd_evnt(client_fd);
        m_client_counts++;

        //把clientfd 添加到任意IO线程里面
        // IOThread* io_thread = m_io_thread_group->getIOThread();
        Eventloop* event_loop = m_io_thread_group->getIOThread()->getEventLoop();

        //TcpConnection objects will be destructed,
        TcpConnection::s_ptr connection= make_shared<TcpConnection>(event_loop, client_fd, 128, peer_addr, m_local_addr);
        connection->setState(Connected);
        m_client.insert(connection);//why TcpConnection objects not be destructed
        INFOLOG("TcpServer succ get client, fd = %d",client_fd);

    }

    void TcpServer::start(){
        m_io_thread_group->start();   //子线程先开启start(),即先开启loop(),这个函数在io_thread中封装的
        m_main_event_loop->loop();    //父线程也开启loop()

    }

    

}