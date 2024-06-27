#pragma once
#ifndef ABSTRACT_CODER_H
#define ABSTRACT_CODER_H

#include<vector>
#include "../tcp/tcp_buffer.h"
#include "abstract_protocol.h" 
namespace rocket{
    class AbstractCode{
    public:

        //编码，将message对象转化为字节流，写入到buffer
        virtual void encode(std::vector<AbstractProtocol::s_ptr>& messages, TcpBuffer::s_ptr out_buffer) = 0;


        //解码，将buffer里面的字节流转化为message对象
        virtual void decode(std::vector<AbstractProtocol::s_ptr>& out_messages, TcpBuffer::s_ptr buffer) = 0;

        virtual ~AbstractCode() {}

    private:

    };



}



#endif