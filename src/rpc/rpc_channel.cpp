#include<google/protobuf/service.h>
#include "../../include/rpc/rpc_channel.h"

namespace rocket
{
    RpcChannel::RpcChannel(NetAddr::s_ptr peer_addr):m_peer_addr(peer_addr){

    }
    void RpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                    google::protobuf::RpcController* controller, const google::protobuf::Message* request,
                    google::protobuf::Message* response, google::protobuf::Closure* done){

                        //发送message对象

                    }
    
} // namespace name
