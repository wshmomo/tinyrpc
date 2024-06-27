
#include<sys/socket.h>
#include <sys/types.h> 
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include "../include/tcp/tcp_client.h"
#include "../include/tcp/net_addr.h"
#include "../include/common/config.h"
#include "../include/common/log.h"
#include "../include/coder/string_coder.h"
#include "../include/coder/abstract_protocol.h"
#include "../include/coder/tinypb_coder.h"
#include "../include/coder/tinypb_protocol.h"
#include "../include/proto/order.pb.h"
#include "../include/rpc/rpc_dispatcher.h"
#include "../include/rpc/rpc_channel.h"
#include "../include/rpc/rpc_controller.h"
#include "../include/rpc/rpc_closure.h"



void test_tcp_client(){
    rocket::IPNetAddr::s_ptr addr = make_shared<rocket::IPNetAddr>("127.0.0.1",8001); 
    rocket::TcpClient client(addr);
    client.connect([&](){
        DEBUGLOG("connect to [%s] success", addr->toString().c_str());
        shared_ptr<rocket::TinyPBProtocol> message = make_shared<rocket::TinyPBProtocol>();
        message->m_msg_id = "99998888";
        message->m_pb_data = "test pb data";

        makeOrderRequest request;
        request.set_price(100);
        request.set_goods("apple");

        //将整个request对象转换成PB对象序列化到message->m_pb_data里面
        if(!request.SerializeToString(&(message->m_pb_data))){
            ERRORLOG("serilize error");
            return;

        }
        message->m_method_name = "Order.makeOrder";

        
        //先发送消息，然后在等其对应的回包，进行读取
        client.writeMessage(message, [request](rocket::AbstractProtocol::s_ptr msg_ptr){
            DEBUGLOG("send message success, request[%s]", request.ShortDebugString().c_str());

        });
        

        client.readMessage("99998888", [](rocket::AbstractProtocol::s_ptr msg_ptr){
            shared_ptr<rocket::TinyPBProtocol> message = dynamic_pointer_cast<rocket::TinyPBProtocol> (msg_ptr); //将父类的智能指针转化为子类的智能指针
            DEBUGLOG("read message success, req_if[%s], get response %s",message->m_msg_id.c_str(), message->m_pb_data.c_str());
            makeOrderResponse reponse;
            if(reponse.ParseFromString(message->m_pb_data)){
                ERRORLOG("deserialize error");
            }
            DEBUGLOG("get response success, reponse[%s]",reponse.ShortDebugString().c_str());




        });
    });


}

void test_rpc_channel(){
    rocket::IPNetAddr::s_ptr addr = make_shared<rocket::IPNetAddr>("127.0.0.1",8001); 
    std::shared_ptr<rocket::RpcChannel> channel = std::make_shared<rocket::RpcChannel>(addr);
    std::shared_ptr<makeOrderRequest> request = std::make_shared<makeOrderRequest>();
    request->set_price(100);
    request->set_goods("apple");
    std::shared_ptr<makeOrderResponse> response = std::make_shared<makeOrderResponse>();

    std::shared_ptr<rocket::RpcController> controller = std::make_shared<rocket::RpcController>();
    controller->SetMsgId("99998888");
     std::shared_ptr<rocket::RpcClosure> closure = std::make_shared<rocket::RpcClosure>([request,response,channel]() mutable{
        INFOLOG("call rpc success, reuqest[%s], response[%s]", request->ShortDebugString().c_str(), response->ShortDebugString().c_str());
        INFOLOG("now exit eventloop");
        channel->getTcpClient()->stop();
        channel.reset();
     });

     channel->Init(controller,request,response,closure);
    
    //客户端的纯根
    Order_Stub stub(channel.get());

    stub.makeOrder(controller.get(),request.get(),response.get(),closure.get());  //这里运行makeOrder就是去执行channel的CallMethod方法


}

int main(){
    rocket::Config::SetGlobalConfig("/home/wsh/code_rpc/tinyrpc/conf/tinyrpc.xml");
    rocket::Logger::InitGlobalLogger();
    // test_tcp_client();
    test_rpc_channel();

    return 0;
    




    // rocket::IPNetAddr addr("127.0.0.1",8001);
    // DEBUGLOG("create addr %s", addr.toString().c_str());
}