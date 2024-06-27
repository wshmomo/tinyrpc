#include<unistd.h>
#include "../../include/tcp/tcp_connection.h"
#include "../../include/common/log.h"
#include "../../include/coder/string_coder.h"
#include "../../include/coder/tinypb_coder.h"
#include "../../include/rpc/rpc_dispatcher.h"



namespace rocket{
    TcpConnection::TcpConnection(Eventloop* eventloop, int fd, int buffer_size, NetAddr::s_ptr peer_addr, NetAddr::s_ptr local_addr,TcpConnectionType type /*= TcpConnectionByServer*/)
      : m_eventloop(eventloop), m_peer_addr(peer_addr), m_local_addr(local_addr), m_state(NotConnected), m_fd(fd), m_connection_type(type){

        m_in_buffer = make_shared<TcpBuffer>(buffer_size);
        m_out_buffer = make_shared<TcpBuffer>(buffer_size);

        m_fd_event = FDEventGroup::getFDEventGroup()->getFDEvent(fd);  //这里的fd究竟是什么，好奇怪，哪部分可以看出clientfd加入了FDEvent里面,在fd_event_group里面的getFDEvent的函数中加入的
        m_fd_event->setNonBlock();

        
        // m_fd_event->listen(FDEvent::IN_EVENT, std::bind(&TcpConnection::OnRead, this));
        // m_eventloop->addEpollEvent(m_fd_event);  
        if(m_connection_type == TcpConnectionByServer){
            listenRead();
        }
        

        // m_coder = new StringCode();
        m_coder = new TinyPBCoder();


        

    }

    TcpConnection::~TcpConnection(){
        DEBUGLOG("~TcpConnection");
        if(m_coder){
            delete m_coder;
            m_coder = NULL;
        }


    }

    void TcpConnection::OnRead(){

        //1.从socket 缓冲区，调用系统的read函数读取字节 in_buffer里面
        if(m_state != Connected){
            ERRORLOG("onRead error, client has already disconnected, addr[%s], clientfd[%d]", m_peer_addr->toString().c_str(), m_fd); //感觉这里有问题clientfd怎么可能这样获取m_fd_event->getFd(),上面解释了
            return;
        }
        bool is_read_all = false;
        bool is_close = false;
        while(!is_read_all){

            if(m_in_buffer->writeAble() == 0){
                m_in_buffer->resizeBuffer(2 * m_in_buffer->m_buffer.size());   //有个问题，这里的buffer的大小的上限是多少？
            }
            int read_count = m_in_buffer->writeAble();
            int write_index = m_in_buffer->writeIndex();


            int rt = read(m_fd, &(m_in_buffer->m_buffer[write_index]), read_count);  //由于read一次最多只能从fd从读read_count个，若fd中还有数据，我们需要继续读
            DEBUGLOG("success read %d bytes from addr[%s],client fd[%d]", rt, m_peer_addr->toString().c_str(),m_fd);
            if(rt > 0){
                m_in_buffer -> moveWriteIndex(rt);

                if(rt == read_count){  //说明缓冲区的数据有我们读的那么多
                    continue;
                }else if( rt < read_count){
                    is_read_all = true;
                    break;

                }
            }else if(rt ==0){
                is_close = true;
                break;
            }else if(rt == -1 && errno == EAGAIN){ //读不到数据了
                is_read_all = true;
                break;

            }
        }

        if(is_close){
            //处理关闭连接
            DEBUGLOG("peer closed, peer addr [%d], clientfd[%d]", m_peer_addr->toString().c_str(), m_fd);
            clear();  //虽然这里clear了，但是由于下面要继续运行，在excute()中虽然删掉了事件，但是m_io_thread->getEventLoop()->addEpollEvent(m_fd_event)，🈶将其添加到事件中了，此时是可读可写事件
            return;   //所以这里我们需要直接返回，这样的话就不用执行下面的excute()了，就不用在添加进去了
            
            
        }

        if(!is_read_all){
            ERRORLOG("not read all data");
        }

        //简单echo，后面补充RPC协议
        excute();

    }

    void TcpConnection::excute(){
        //执行RPC请求，获取RPC响应，再把RPC响应发送回去
        if(m_connection_type == TcpConnectionByServer){
            // vector<char> tmp;
            // int size = m_in_buffer->readAble();
            // tmp.resize(size);
            // m_in_buffer->readFromBuffer(tmp,size);
            // string msg;
            // for(int i = 0; i < tmp.size(); i++){
            //     msg += tmp[i];
            // }
            std::vector<AbstractProtocol::s_ptr> result;
            std::vector<AbstractProtocol::s_ptr> replay_messages;
            m_coder->decode(result, m_in_buffer);
            for(int i =0; i < result.size(); i++){
                //1.针对每一个请求，调用rpc方法，获取响应message
                //2.将响应message放到发送缓冲区，监听可写事件回包
                INFOLOG("success get request[%s] from client[%s]", result[i]->m_req_id.c_str(),m_peer_addr->toString().c_str());
                std::shared_ptr<TinyPBProtocol> message = std::make_shared<TinyPBProtocol>();
                // message->m_pb_data = "hello. this is rocket rpc test data";
                // message->m_req_id = result[i]->m_req_id;
                RpcDispatcher::getRpcDispatcher()->dispatch(result[i],message,this);
                replay_messages.emplace_back(message);

            }
            m_coder->encode(replay_messages,m_out_buffer);
            // m_out_buffer->wirteToBuffer(msg.c_str(), msg.length());
            // INFOLOG("success get request[%s] from client[%s]", result[i]->m_req_id.c_str(),m_peer_addr->toString().c_str());


            

            // m_fd_event->listen(FDEvent::OUT_EVENT,std::bind(&TcpConnection::OnWrite,this));
            // m_eventloop->addEpollEvent(m_fd_event);

            listenWrite();

        }else{
            //从buffer里done得到 message对象, 判断是否req_id相等，相等读成功, 执行其回调

            vector<AbstractProtocol::s_ptr> result;
            m_coder->decode(result,m_in_buffer);
            for(int i = 0; i < m_read_dones.size(); i++){
                string req_id = result[i]->m_req_id;
                auto it = m_read_dones.find(req_id);
                if( it != m_read_dones.end()){
                    it->second(result[i]);  //result[i]->shared_from_this()：从这个对象中获取其智能指针
                }
            }
        }

        

    }

    void TcpConnection::OnWrite(){
        // 将当前 out_buffer里面的数据全部发送给client
        if(m_state != Connected){
            ERRORLOG("onWrite error, client has already disconnected, addr[%s], clientfd[%d]", m_peer_addr->toString().c_str(), m_fd);
            return;
        }

        if(m_connection_type == TcpConnectionByClient){
            //1.将message encode得到字节流
            //2.将字节流写入到buffer里面,然后全部发送

            std::vector<AbstractProtocol::s_ptr> messages;
            

            for(int i =0; i < m_write_dones.size(); i++){
                messages.push_back(m_write_dones[i].first);  //这里的智能指针用get()以取得裸指针
            }

            m_coder->encode(messages, m_out_buffer);

 
            

        }


        bool is_write_all = false;
        while(true){
            if(m_out_buffer->readAble() == 0){
                DEBUGLOG("no data need to send to client[%s]", m_peer_addr->toString().c_str());
                is_write_all = true;
                break;
            }

            int write_size = m_out_buffer->readAble();
            int read_index = m_out_buffer->readIndex();

            int rt = write(m_fd, &(m_out_buffer->m_buffer[read_index]), write_size);

            if(rt >= write_size){
                DEBUGLOG("no data need to send to client[%s]", m_peer_addr->toString().c_str());
                is_write_all = true;
                break;

            }else if(rt == -1 && errno == EAGAIN){//说明m_fd的缓冲区已满，不能在发送(对于非阻塞的情况)
                //需要等下次m_fd可写
                ERRORLOG("write data error, errno == EAGAIN and rt == -1");
                break;


            }
        }
        if(is_write_all){
            m_fd_event->cancle(FDEvent::OUT_EVENT);
            m_eventloop->addEpollEvent(m_fd_event); //because read event need still listen 
        }

        if(m_connection_type == TcpConnectionByClient){
            for(int i =0; i < m_write_dones.size(); i++){
                m_write_dones[i].second(m_write_dones[i].first);
            }
            m_write_dones.clear();
        }

        

    }

    void TcpConnection::setState(const TcpState state){
        m_state = Connected;


    }

    TcpState TcpConnection::getState(){

        return m_state;
    }


    void TcpConnection::clear(){
        //代表处理一些关闭连接后的清理动作

        if(m_state == Closed){
            return;
        }

        m_fd_event->cancle(FDEvent::IN_EVENT);  //断开读操作
        m_fd_event->cancle(FDEvent::OUT_EVENT); //断开写操作

        m_eventloop->deleteEpollEvent(m_fd_event);  //子线程的
        m_state = Closed;

    }


    void TcpConnection::shutdown(){
        if(m_state == Closed || m_state == NotConnected){
            return;
        }

        //处于半关闭状态
        m_state = HalfClosing;

        //调用shuntdown关闭读写，意味着服务器不会再对这个fd进行读写操作
        //触发了四次挥手的第一个阶段，发送FIN报文，此时连接还没关闭，等到接收到对端返回的电报才会关闭，即当fd发生可读事件，但是可读的数据为0,即对端发送了FIN
        ::shutdown(m_fd,SHUT_RDWR);
    }

    void TcpConnection::setConnectionType(TcpConnectionType type){
        m_connection_type = type;

    }

    void TcpConnection::listenWrite(){
        m_fd_event->listen(FDEvent::OUT_EVENT,std::bind(&TcpConnection::OnWrite,this));
        m_eventloop->addEpollEvent(m_fd_event);
        
    }


    void TcpConnection::listenRead(){

        m_fd_event->listen(FDEvent::IN_EVENT, std::bind(&TcpConnection::OnRead, this));
        m_eventloop->addEpollEvent(m_fd_event);   

    }

    void TcpConnection::pushSendMessage(AbstractProtocol::s_ptr message,function<void(AbstractProtocol::s_ptr)> done){
        m_write_dones.push_back(pair(message,done));
    }

    void TcpConnection::pushReadMessage(const string& req_id,function<void(AbstractProtocol::s_ptr)> done){
        m_read_dones.insert(make_pair(req_id,done));

    }


    NetAddr::s_ptr TcpConnection::getLocaladdr(){
        return m_local_addr;

    }

    NetAddr::s_ptr TcpConnection::getPeeraddr(){
        return m_peer_addr;

    }
}