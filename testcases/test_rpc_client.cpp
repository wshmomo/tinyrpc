
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



void test_tcp_client(){
    rocket::IPNetAddr::s_ptr addr = make_shared<rocket::IPNetAddr>("127.0.0.1",8001); 
    rocket::TcpClient client(addr);
    client.connect([&](){
        DEBUGLOG("connect to [%s] success", addr->toString().c_str());
        shared_ptr<rocket::TinyPBProtocol> message = make_shared<rocket::TinyPBProtocol>();
        message->m_req_id = "99998888";
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
            DEBUGLOG("read message success, req_if[%s], get response %s",message->m_req_id.c_str(), message->m_pb_data.c_str());
            makeOrderResponse reponse;
            if(reponse.ParseFromString(message->m_pb_data)){
                ERRORLOG("deserialize error");
            }
            DEBUGLOG("get response success, reponse[%s]",reponse.ShortDebugString().c_str());

        });
    });


}

int main(){
    rocket::Config::SetGlobalConfig("/home/wsh/code_rpc/tinyrpc/conf/tinyrpc.xml");
    rocket::Logger::InitGlobalLogger();
    test_tcp_client();

    return 0;
    




    // rocket::IPNetAddr addr("127.0.0.1",8001);
    // DEBUGLOG("create addr %s", addr.toString().c_str());
}