
#include<google/protobuf/service.h>
#include "../include/common/config.h"
#include "../include/common/log.h"
#include "../include/tcp/net_addr.h"
#include "../include/tcp/tcp_server.h"
#include "../include/proto/order.pb.h"
#include "../include/rpc/rpc_dispatcher.h"

class OrderImpl : public Order{
public:
   //简单的业务逻辑
    void makeOrder(google::protobuf::RpcController* controller,
                       const ::makeOrderRequest* request,
                       ::makeOrderResponse* response,
                       ::google::protobuf::Closure* done){

                        DEBUGLOG("start sleep 5s");
                        sleep(5);
                        DEBUGLOG("end sleep 5s");
                        if(request->price() < 10){
                            response->set_ret_code(-1);
                            response->set_res_info("short balance");
                            return;
                        }
                        response->set_order_id("20240626");

                       }

};

void test_tcp_server(){
    rocket::IPNetAddr::s_ptr addr = make_shared<rocket::IPNetAddr>("127.0.0.1",8001);  //这是一个IPNetAddr类指针
    DEBUGLOG("create addr %s", addr->toString().c_str());
    rocket::TcpServer tcp_server(addr);
    tcp_server.start();

}

int main(){
    rocket::Config::SetGlobalConfig("/home/wsh/code_rpc/tinyrpc/conf/tinyrpc.xml");
    rocket::Logger::InitGlobalLogger();
    std::shared_ptr<OrderImpl> service = std::make_shared<OrderImpl>();
    rocket::RpcDispatcher::getRpcDispatcher()->registerService(service);  //这里表示注册了，你这里是根据那个order.proto得到的，所以注册的名字也是从那里得到的
    test_tcp_server();


    return 0;
    




    // rocket::IPNetAddr addr("127.0.0.1",8001);
    // DEBUGLOG("create addr %s", addr.toString().c_str());
}