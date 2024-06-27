#include "../../include/tcp/net_addr.h"
#include<string.h>


namespace rocket{

    IPNetAddr::IPNetAddr(const string& ip, uint16_t port): m_ip(ip), m_port(port){
        memset(&m_addr,0,sizeof(m_addr));

        m_addr.sin_family = AF_INET;
        m_addr.sin_addr.s_addr = inet_addr(m_ip.c_str());  //这个只适用于IPV4
        m_addr.sin_port = htons(m_port);

        // inet_pton(AF_INET, m_ip.c_str(), (void*) &m_addr.sin_addr.s_addr); //这个适用于IPV4和IPV6


    }

    IPNetAddr::IPNetAddr(const string& addr){
        size_t i = addr.find_first_of(":");
        if(i == addr.npos){
            ERRORLOG("invalid ipv4 addr %s", addr.c_str());
            return;
        }
        m_ip = addr.substr(0,i);
        m_port = atoi(addr.substr(i+1, addr.size()-i-1).c_str());


        memset(&m_addr,0,sizeof(m_addr));

        m_addr.sin_family = AF_INET;
        m_addr.sin_addr.s_addr = inet_addr(m_ip.c_str());  //这个只适用于IPV4
        m_addr.sin_port = htons(m_port);

    }

    IPNetAddr::IPNetAddr(sockaddr_in addr) : m_addr(addr){
        m_ip = string(inet_ntoa(m_addr.sin_addr)); // 将网络ip形式转换成ASCII码,然后再转换成string的类型
        m_port = ntohs(addr.sin_port);
        

    }


    sockaddr* IPNetAddr::getSockAddr(){
        return (sockaddr*) &m_addr;

    }

    socklen_t IPNetAddr::getSockLen(){

        return sizeof(m_addr);

    }

    int IPNetAddr::getFamily(){

        return AF_INET;

    }

    string IPNetAddr::toString(){
        string re;
        re = m_ip + ":" + to_string(m_port);
        return re;
    }


    bool IPNetAddr::checkValid(){
        if(m_ip.empty()){
            return false;
        }
        if(m_port < 0 || m_port > 65535){ //这里端口号最大是65536-1 =  2^16-1
            return false;

        }
        if(inet_addr(m_ip.c_str()) == INADDR_NONE){
            return false;
        }

        return true;

    }
}