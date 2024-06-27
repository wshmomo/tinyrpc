#pragma once
#ifndef ABSTRACT_PROTOCOL_H
#define ABSTRACT_PROTOCOL_H

#include<memory>
#include<string>

//协议

namespace rocket{
    //只有继承了才能获取其只能指针？why?只有这样才能安全的访问其shared_ptr
    class AbstractProtocol : public std::enable_shared_from_this<AbstractProtocol>{
    public:
        typedef std::shared_ptr<AbstractProtocol> s_ptr;

        // std::string getMsgId(){
        //     return m_msg_id;
        // }

        // void setMsgId(const std::string& id){
        //     m_msg_id = id;
        // }

        virtual ~AbstractProtocol() {}


    public:
        std::string m_msg_id;  //请求号，唯一标识一个请求或者响应

    private:
    };


    
}


#endif