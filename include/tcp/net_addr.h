#pragma once
#ifndef NETADDR_H
#define NETADDR_H

#include<arpa/inet.h>
#include<netinet/in.h>
#include<memory>
#include<string>
#include "../common/log.h"

namespace rocket{
    class NetAddr{
    public:

        typedef shared_ptr<NetAddr> s_ptr;
        //定义一个抽象类,这里为什么定义成一个抽象类，因为不同的应用得到的地址不太一样
        virtual sockaddr* getSockAddr() = 0;

        virtual socklen_t getSockLen() = 0;

        virtual int getFamily() = 0;


        virtual string toString() = 0;

        virtual bool checkValid() = 0;

    private:

    };


    class IPNetAddr: public NetAddr{
    public:

        IPNetAddr(const string& ip, uint16_t port);

        IPNetAddr(const string& addr);

        IPNetAddr(sockaddr_in addr);


        sockaddr* getSockAddr();

        socklen_t getSockLen();

        int getFamily();

        string toString();

        bool checkValid();
        
    private:
        string m_ip;
        uint16_t m_port {0};

        sockaddr_in m_addr;

    };
}


#endif