#include<sys/socket.h>
#include <sys/types.h> 
#include<string.h>
#include "../../include/tcp/tcp_acceptor.h"

namespace rocket{

    TcpAcceptor::TcpAcceptor(NetAddr::s_ptr local_addr) : m_local_addr(local_addr){

        if(!local_addr->checkValid()){
            ERRORLOG("invalid local addr %s", local_addr->toString().c_str());
            exit(0);
        }
        m_family = m_local_addr->getFamily();
        m_listenfd = socket(m_family,SOCK_STREAM,0);

        if(m_listenfd < 0){
            ERRORLOG("invalid listenfd %d", m_listenfd);
            exit(0);
        }

        //端口复用

        int val = 1;
        int rt = setsockopt(m_listenfd, SOL_SOCKET, SO_REUSEADDR, (void*) &val, sizeof(val));
        if(rt != 0){
            ERRORLOG("setsockopt REUSEADDR error, errno = %d, error = %s",errno,strerror(errno));
        }

        socklen_t len = m_local_addr->getSockLen();
        if((bind(m_listenfd,m_local_addr->getSockAddr(),len)) != 0){
            ERRORLOG("bind error,errno = %d, error = %s",errno,strerror(errno));
            exit(0);
        }

        if((listen(m_listenfd,1000)) != 0){
            ERRORLOG("listen error,errno = %d, error = %s",errno,strerror(errno));
            exit(0);
        }

        //这里需要把监听套接字设置成非阻塞的，因为eventloop很多地方用的是while循环来读，当做非阻塞的来读，一旦你的套接字是阻塞的话，你读的字节数比他当前缓冲区的字节大，读取就会阻塞，
        //在主从rector模型中，不允许这种情况发生，我们希望我们所有的操作都是异步完成的，如果缓冲区的字节数不足，我们希望不要阻塞，异步的去读


        

    }
        
    TcpAcceptor:: ~TcpAcceptor(){

    }
   
    pair<int,NetAddr::s_ptr> TcpAcceptor::accept(){
        if(m_family == AF_INET){
            sockaddr_in client_addr;
            memset(&client_addr,0,sizeof(client_addr));
            socklen_t client_addr_len = sizeof(client_addr);
            
            //这里加两个冒号，代表调用系统的函数
            int clientfd = ::accept(m_listenfd,(sockaddr*) &client_addr,&client_addr_len);
            if(clientfd < 0){
                ERRORLOG("accept error,errno = %d, error = %s",errno,strerror(errno));
                exit(0);
            }
            IPNetAddr::s_ptr peer_addr= make_shared<IPNetAddr>(client_addr);
            INFOLOG("A client have accepted sucess, peer addr[%s]",peer_addr->toString().c_str());
            return make_pair(clientfd,peer_addr);

        }else{
            //
            return make_pair(-1,nullptr);
        }

    }
}