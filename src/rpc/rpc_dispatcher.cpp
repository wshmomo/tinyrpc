#include<google/protobuf/service.h>
#include<google/protobuf/descriptor.h>
#include<google/protobuf/message.h>
#include<memory>
#include "../../include/rpc/rpc_dispatcher.h"
#include "../../include/coder/tinypb_protocol.h"
#include"../../include/common/log.h"
#include "../../include/common/error_code.h"
#include "../../include/rpc/rpc_controller.h"
#include "../../include/tcp/tcp_connection.h"

namespace rocket{

    static RpcDispatcher* g_rpc_dispatcher = NULL;
    RpcDispatcher*  RpcDispatcher::getRpcDispatcher(){
        if(g_rpc_dispatcher != NULL){
            return g_rpc_dispatcher;
        }
        
        g_rpc_dispatcher = new RpcDispatcher();
        return g_rpc_dispatcher;
    }
    void RpcDispatcher::dispatch(AbstractProtocol::s_ptr request, AbstractProtocol::s_ptr response, TcpConnection* connection){
        //1. 注册OrderService对象
        //2. 从buffer读取数据，然后decode得到请求的TinyPBProtobol对象，然后从请求的TinyPBProtocol得到 method_name, 从OrderService对象里根据service.method_name找到方法func
        //3. 找到对应的request type以及 response type
        //4. 将请求体里面的pb_data反序列化为request type的一个对象，声明一个空的reponse type对象
        //5. func(request, reponse)，执行业务逻辑
        //6. 将response对象序列化为pb_data，在塞到TinyPBProtocol对象中。在做encode 然后塞到buffer里面，就会发送回包了。
        std::shared_ptr<TinyPBProtocol> req_protocol = std::dynamic_pointer_cast<TinyPBProtocol>(request);
        std::shared_ptr<TinyPBProtocol> rsp_protocol = std::dynamic_pointer_cast<TinyPBProtocol>(response);

        std::string method_full_name = req_protocol->m_method_name;
        std::string service_name;
        std::string method_name;

        rsp_protocol->m_msg_id =  req_protocol->m_msg_id;
        rsp_protocol->m_method_name = req_protocol->m_method_name;

        if(!parseServiceFullName(method_full_name,service_name,method_name)){
            
            setTinyPBError(rsp_protocol,ERROR_PARSE_SERVICE_NAME, "parse service name error");
            return;

        }

        auto it = m_service_map.find(service_name);
        if(it == m_service_map.end()){
            ERRORLOG("%s | service name[%s] not found", req_protocol->m_msg_id.c_str(), service_name.c_str());
            setTinyPBError(rsp_protocol,ERROR_SERVICE_NOT_FOUND, "service not find");
            return;
        }

        service_s_ptr service = (*it).second;
        const google::protobuf::MethodDescriptor* method  = service->GetDescriptor()->FindMethodByName(method_name);  //makeOrder，方法的名字，也就是业务
        if(method == NULL){
            ERRORLOG("%s | method name[%s] not found in service[%s]", req_protocol->m_msg_id.c_str(), method_name.c_str(), service_name.c_str());
            setTinyPBError(rsp_protocol,ERROR_SERVICE_NOT_FOUND, "method not find");
            return;            
        }
        google::protobuf::Message* req_msg = service->GetRequestPrototype(method).New();  //从业务中方法中得到请求

        //反序列化，将pb_data反序列化为message，也就是反序列化成protobuf类型的东西
         //注意这里是调用的protocol里面的
        if(req_msg->ParseFromString(req_protocol->m_pb_data)){
            ERRORLOG("%s | deserilize error", req_protocol->m_msg_id.c_str(), method_name.c_str(), service_name.c_str());
            setTinyPBError(rsp_protocol,ERROR_FAILED_DESERIALIZE, "deserilize error");
            if(req_msg != NULL){
                delete req_msg;
                req_msg = NULL;

            }
            return;  
        }

        INFOLOG("msg_id[%s], get rpc request[%s]", req_protocol->m_msg_id.c_str(), req_msg->ShortDebugString().c_str());

        google::protobuf::Message* rsp_msg = service->GetResponsePrototype(method).New();  //从业务中方法中得到的reponse

        RpcController rpccontroller;


        rpccontroller.SetLocalAddr(connection->getLocaladdr());
        rpccontroller.SetPeerAddr(connection->getPeeraddr());
        rpccontroller.SetMsgId(req_protocol->m_msg_id);


        service->CallMethod(method,&rpccontroller,req_msg,rsp_msg,NULL);   


        //将这个rsp_msg对象序列化，然后写入到rsp_protocol->m_pb_data
        if(!rsp_msg->SerializeToString(&(rsp_protocol->m_pb_data))){
            ERRORLOG("%s | serilize error, origin message [%s]", req_protocol->m_msg_id.c_str(), rsp_msg->ShortDebugString().c_str());
            setTinyPBError(rsp_protocol,ERROR_FAILED_SERIALIZE, "serilize error");
            if(req_msg != NULL){
                delete req_msg;
                req_msg = NULL;

            }
            if(rsp_msg != NULL){
                delete rsp_msg;
                rsp_msg = NULL;
            }
            return;              
        }

        rsp_protocol->m_err_code = 0;

        INFOLOG("%s | dispatch success, request[%s], reponse[%s]", req_protocol->m_msg_id.c_str(), req_msg->ShortDebugString().c_str(), rsp_msg->ShortDebugString().c_str());
        delete req_msg;
        delete rsp_msg;
        req_msg = NULL;
        rsp_msg = NULL;

    }

    bool RpcDispatcher::parseServiceFullName(const std::string& full_name, std::string& service_name, std::string& method_name){
        if(full_name.empty()){
            ERRORLOG("full name empty");
            return false;
        }
        size_t i =full_name.find_first_of(".");
        if(i == full_name.npos){
            ERRORLOG("not find . in full name [%s]", full_name);
            return false;
        }

        service_name = full_name.substr(0,i);
        method_name = full_name.substr(i+1, full_name.length() - i -1);
        INFOLOG("parse service_name[%s] and method_name[%s] from full_name[%s]",service_name.c_str(),method_name.c_str(),full_name.c_str());
        return true;
        
    }

    void RpcDispatcher::registerService(service_s_ptr service){
        std::string service_name = service->GetDescriptor()->full_name();  //为什么这里可以这样用？
        m_service_map[service_name] = service;

    }

    void RpcDispatcher::setTinyPBError(std::shared_ptr<TinyPBProtocol> msg, int32_t err_code, const std::string err_info){
        msg->m_err_code = err_code;
        msg->m_err_info = err_info;
        msg->m_err_info_len = err_info.length();
    }




}