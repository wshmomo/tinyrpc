#ifndef STRING_CODER_H
#define STRING_CODER_H

#include "abstract_protocol.h"
#include "abstract_coder.h"
namespace rocket{
    class StringProtocol: public AbstractProtocol{
        public:
            string info;

        

    };

    class StringCode : public AbstractCode{
    public:
        //编码，将message对象转化为字节流，写入到buffer
        void encode(std::vector<AbstractProtocol::s_ptr>& messages, TcpBuffer::s_ptr out_buffer){
            // string msg = "hello rocket";
            // out_buffer->wirteToBuffer(msg.c_str(), msg.length());
            for(int i =0; i < messages.size(); i++){
                shared_ptr<StringProtocol> msg = dynamic_pointer_cast<StringProtocol>(messages[i]);  //将父类指针转换成子类指针
                out_buffer->wirteToBuffer(msg->info.c_str(),msg->info.length());
            }
        }


        //解码，将buffer里面的字节流转化为message对象
        void decode(std::vector<AbstractProtocol::s_ptr>& out_messages, TcpBuffer::s_ptr buffer){
            std::vector<char> re;
            buffer->readFromBuffer(re, buffer->readAble());  //从m_buffer中读数据，读到re中，将全部可读的都读进去
            string info;
            for (size_t i = 0; i < re.size(); i++)
            {
                info += re[i];
            }
            // StringProtocol* msg = new StringProtocol();
            shared_ptr<StringProtocol> msg = make_shared<StringProtocol>();
            msg->info = info;
            msg->m_msg_id = "123456";  //其实这里还有个逻辑问题，你为什么可以知道这个包的名称，我们需要去进行查询，而不是进行直接设置
            out_messages.push_back(msg);
            
        }


    private:
    };

}


#endif