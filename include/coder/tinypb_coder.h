#ifndef TINYPB_CODER_H
#define TINYPB_CODER_H
#include "abstract_coder.h"
#include "tinypb_protocol.h"

namespace rocket{
    class TinyPBCoder : public AbstractCode{
    public:

        TinyPBCoder(){}
        ~TinyPBCoder(){}
        //编码，将message对象转化为字节流，写入到buffer
        void encode(std::vector<AbstractProtocol::s_ptr>& messages, TcpBuffer::s_ptr out_buffer);


        //解码，将buffer里面的字节流转化为message对象
        void decode(std::vector<AbstractProtocol::s_ptr>& out_messages, TcpBuffer::s_ptr buffer);


    private:
        //将单个包转换成字节流
        const char* encodeTinyPB(std::shared_ptr<TinyPBProtocol> message, int& len);
    };
}


#endif