#ifndef RPC_CLOSURE_H
#define RPC_CLOSURE_H

#include<google/protobuf/stubs/callback.h>
#include<functional>
namespace rocket{
     class RpcClosure : public google::protobuf::Closure{
    public:
        RpcClosure(std::function<void()> cb): m_cb(cb){}
        ~RpcClosure(){}

        void Run(){
            if(m_cb != nullptr){
                m_cb();
            }

        }


        std::function<void()> m_cb {nullptr};


     };
}


#endif