#include<google/protobuf/service.h>
#include<google/protobuf/descriptor.h>
#include<google/protobuf/message.h>
#include "../../include/rpc/rpc_channel.h"
#include "../../include/coder/tinypb_protocol.h"
#include "../../include/common/msg_id_util.h"
#include "../../include/rpc/rpc_controller.h"
#include "../../include/common/log.h"
#include "../../include/tcp/tcp_client.h"
#include "../../include/common/error_code.h"
#include "../../include/net/timer_event.h"

namespace rocket
{
    RpcChannel::RpcChannel(NetAddr::s_ptr peer_addr):m_peer_addr(peer_addr){
        m_client = std::make_shared<TcpClient>(m_peer_addr);

    }
    RpcChannel::~RpcChannel(){
        INFOLOG("~RpcChannel");

     }
    //注意这里还有个回调函数done,他的含义是若我们调用本次RPC成功就执行这个回调函数，如何判断其调用RPC是否成功，看能否收到m_msg_id对应的回包
    void RpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                    google::protobuf::RpcController* controller, const google::protobuf::Message* request,
                    google::protobuf::Message* response, google::protobuf::Closure* done){

        //发送message对象
        std::shared_ptr<TinyPBProtocol> req_protocol = std::make_shared<TinyPBProtocol>();
        RpcController* my_controller = (RpcController*)(controller);
        if(my_controller == nullptr){
            ERRORLOG("failed callmethod, RpcController convert error");
            return;
        }



        if(my_controller->GetMsgId().empty()){
            req_protocol->m_msg_id = MsgIDUtil::GetMsgID();
        }else{
            req_protocol->m_msg_id = my_controller->GetMsgId();
        }

        req_protocol->m_method_name = method->full_name();  //这里的full那么是啥？
        INFOLOG("%s | call method name[%s]", req_protocol->m_msg_id.c_str(), req_protocol->m_method_name.c_str());


        if(!m_is_init){
            ERRORLOG("RpcChannle not init");
            std::string err_info = "RpcChannle not init";
            my_controller->SetError(ERROR_RPC_CHANNEL_INIT,err_info);
            ERRORLOG("%s | %s, RpcChannle not init", req_protocol->m_msg_id.c_str(), err_info.c_str());
            return;
        }

        //requeset 的序列化
        if(!request->SerializeToString(&(req_protocol->m_pb_data))){
            std::string err_info = "failed to serialize";
            my_controller->SetError(ERROR_FAILED_SERIALIZE,err_info);
            ERRORLOG("%s | %s, origin request[%s]", req_protocol->m_msg_id.c_str(), err_info.c_str(),request->ShortDebugString().c_str());
            return;
        }


         s_ptr channel = shared_from_this();  //获取当前类的智能指针
        //在此之前成功构造了个请求的协议对象（TinyPBrotocol类型 req_protocol）


        //1.设置超时； 2.将超时任务添加到epoll里面
        m_timer_event = std::make_shared<TimerEvent>(my_controller->GetTimeout(),false, [my_controller,channel]() mutable{
            my_controller->StartCancel();
            my_controller->SetError(ERROR_RPC_CALL_TIMEOUT, "rpc call timeout" + std::to_string(my_controller->GetTimeout()));
            if(channel->getClosure()){
                channel->getClosure()->Run();
            }

            channel.reset();


        });

        m_client->addTimerEvent(m_timer_event);

        m_client->connect([req_protocol,channel]()mutable{
            RpcController* my_controller = (RpcController*)(channel->getController());
            if(channel->getTcpClient()->getConnectErrorCode() != 0){
                ERRORLOG("%s | connect error, error code[%d], error info[%s], peer addr[%s]", req_protocol->m_msg_id.c_str(),channel->getTcpClient()->getConnectErrorCode(),
                         channel->getTcpClient()->getConnectErrorInfo().c_str(), channel->getTcpClient()->getPeerAddr()->toString().c_str());
                my_controller->SetError(channel->getTcpClient()->getConnectErrorCode(), channel->getTcpClient()->getConnectErrorInfo());
                return;

            }
            channel->getTcpClient()->writeMessage(req_protocol,[req_protocol,channel](AbstractProtocol::s_ptr msg){//协程写到这里就不用那么多回调函数了？整体代码看起来是同步的代码，但是性能是异步的性能。
                INFOLOG("%s |  send request success, call method name[%s], peer addr[%s], local addr[%s]",req_protocol->m_msg_id.c_str(), 
                        req_protocol->m_method_name.c_str(), channel->getTcpClient()->getPeerAddr()->toString().c_str(),
                        channel->getTcpClient()->getLocalAddr()->toString().c_str());

            });

            channel->getTcpClient()->readMessage(req_protocol->m_msg_id,[channel,my_controller](AbstractProtocol::s_ptr msg) mutable{
                std::shared_ptr<TinyPBProtocol> rsp_protocol = std::dynamic_pointer_cast<TinyPBProtocol>(msg);
                INFOLOG("%s | sucess get rpc response, call method name[%s], peer addr[%s], local addr[%s]", rsp_protocol->m_msg_id.c_str(), 
                        rsp_protocol->m_method_name.c_str(),channel->getTcpClient()->getPeerAddr()->toString().c_str(),
                        channel->getTcpClient()->getLocalAddr()->toString().c_str());
                std::shared_ptr<TinyPBProtocol> req_protocol = std::make_shared<TinyPBProtocol>();
                
                //读包成功说明rpc call成功，所以我们需要删除这个定时任务
                channel->getTimerEvent()->setCancled(true);

                //回包之后没有反序列化，这里需要反序列化一下子
                if(channel->getResponse()->ParseFromString(rsp_protocol->m_pb_data)){
                    ERRORLOG("%s | deserialize error", rsp_protocol->m_msg_id.c_str());
                    my_controller->SetError(ERROR_FAILED_DESERIALIZE,"deserialize error");
                    return;
                }
                if(req_protocol->m_err_code != 0){
                    ERRORLOG("%s | call rpc method[%s] failed, error code[%s], error info[%s]", rsp_protocol->m_msg_id.c_str(), rsp_protocol->m_method_name.c_str(),rsp_protocol->m_err_info.c_str());
                    my_controller->SetError(rsp_protocol->m_err_code, rsp_protocol->m_err_info);
                    return;

                }

                INFOLOG("%s | call rpc success, call method name[%s],peer addr[%s], local addr[%s]",
                       rsp_protocol->m_msg_id.c_str(), rsp_protocol->m_method_name.c_str(),
                       channel->getTcpClient()->getPeerAddr()->toString().c_str(),
                        channel->getTcpClient()->getLocalAddr()->toString().c_str());


                
                if(channel->getClosure()){
                    channel->getClosure()->Run();
                }
                channel.reset();  //为什么不加mutable这里会报错，这里reset就会给channel的智能指针减一，如果外层的channel智能指针已经没有了，那么channel对象就会正常的析构，避免内存泄漏，但是有个问题

            });

        });

    }

    void RpcChannel::Init(controller_s_ptr controller, message_s_ptr request, message_s_ptr response, closure_s_ptr closure){
        if(m_is_init){
            return;
        }

        m_controller =controller;  //这里是为了保存这些东西，不让他中途析构，等channel结束之后，利用channel.reset()慢慢析构
        m_request = request;
        m_response =response;
        m_closure = closure;
        m_is_init = true;

    }

    google::protobuf::RpcController* RpcChannel::getController(){
        return m_controller.get();

    }
    google::protobuf::Message* RpcChannel::getRequest(){
        return m_request.get();

    }
    google::protobuf::Message* RpcChannel::getResponse(){
        return m_response.get();

    }
    google::protobuf::Closure* RpcChannel::getClosure(){
        return m_closure.get();

    }

    TcpClient* RpcChannel::getTcpClient(){
        return m_client.get();
    }

    TimerEvent* RpcChannel::getTimerEvent(){
        return m_timer_event.get();
    }
    
} // namespace name
