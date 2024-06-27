#include "../../include/tcp/tcp_client.h"
#include "../../include/common/log.h"
#include "../../include/net/eventloop.h"
#include "../../include/net/fd_event_group.h"

namespace rocket{
    TcpClient::TcpClient(NetAddr::s_ptr peer_addr): m_peer_addr(peer_addr){
        m_event_loop = Eventloop::getCurrentEventloop();  //每个线程一个eventloop事件
        m_fd = socket(peer_addr->getFamily(),SOCK_STREAM,0);

        if(m_fd < 0){
            ERRORLOG("TcpClient::TcpClient() error, failed to create fd");
        }

        m_fd_event = FDEventGroup::getFDEventGroup()->getFDEvent(m_fd);
        m_fd_event->setNonBlock();
        m_connection = make_shared<TcpConnection>(m_event_loop,m_fd,128,peer_addr, nullptr, TcpConnectionByClient);
        m_connection->setConnectionType(TcpConnectionByClient);

    }

    TcpClient::~TcpClient(){
        if(m_fd > 0){
            close(m_fd);
        }

    }
    //异步的进行connect
    //如果connect完成，done会被执行（connect成不成功要看错误码)
    void TcpClient::connect(std::function<void()> done){
        int rt = ::connect(m_fd, m_peer_addr->getSockAddr(),m_peer_addr->getSockLen());
        if(rt == 0){
            DEBUGLOG("connect success");
            if(done){
                done();
            }
        }else if(rt == -1){
            if(errno == EINPROGRESS){//当你在一个非阻塞（non-blocking）套接字上调用 connect 函数时，通常情况下，如果连接无法立即建立，connect 函数会返回 -1，并设置 errno 为 EINPROGRESS。
                //这表明连接请求已经发出，但是连接过程还没有完成。换句话说，连接正在进行中，你需要等待连接完成
                //epoll 监听可写事件，然后判断错误码
                m_fd_event->listen(FDEvent::OUT_EVENT, [this,done](){
                    int error = 0;
                    socklen_t error_len = sizeof(error);
                    getsockopt(m_fd, SOL_SOCKET, SO_ERROR,&error, &error_len); //通过 getsockopt 获取套接字的错误状态，这是一个常见的方式来检查非阻塞连接操作的结果
                    bool is_connect_succ = false;
                    if(error == 0){
                        DEBUGLOG("connect [%s] success", m_peer_addr->toString().c_str());
                        is_connect_succ = true;
                        m_connection->setState(Connected);
                    
                    }else{
                        ERRORLOG("connect error, errno = %d, error = %s",errno,strerror(errno));
                    }

                    //这里还需要关闭可写事件，要不然就会一直触发可写事件
                    m_fd_event->cancle(FDEvent::OUT_EVENT);
                    m_event_loop->addEpollEvent(m_fd_event);

                    if(is_connect_succ && done){
                        done();
                    }  //开始的时候放在cancle之前，会出现时序问题，这样OUT_EVENT被结束掉，根本不会触发写事件
                });
                m_event_loop->addEpollEvent(m_fd_event);

                if(!m_event_loop->isLooping()){
                    m_event_loop->loop();  
                }
                
            }else{
                ERRORLOG("connect error, errno = %d, error = %s",errno,strerror(errno));
            }
        }
        // if(!m_event_loop->isLooping()){
        //         m_event_loop->loop();  
        // }
    }

    void TcpClient::stop(){
        if(m_event_loop->isLooping()){
            m_event_loop->stop();
        }
    }
        

    //异步的发送Message
    //如果发送Message成功，会调用done函数， 函数的入参就是message对象
    void TcpClient::writeMessage(AbstractProtocol::s_ptr request, std::function<void(AbstractProtocol::s_ptr )> done){
        //1.把message对象写入到connection的buffer，done也写入
        //2. 启动connection可写事件
        m_connection->pushSendMessage(request,done);
        m_connection->listenWrite();

    }

    void TcpClient::readMessage(const string& msg_id, std::function<void(AbstractProtocol::s_ptr )> done){
        //1.监听可读事件
        //2.从buffer里done得到 message对象, 判断是否msg_id相等，相等读成功, 执行其回调
        m_connection->pushReadMessage(msg_id,done);
        m_connection->listenRead();

    }
}