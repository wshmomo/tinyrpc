#include "../include/common/config.h"
#include "../include/common/log.h"
#include "../include/tcp/net_addr.h"
#include "../include/tcp/tcp_server.h"

void test_tcp_server(){
    rocket::IPNetAddr::s_ptr addr = make_shared<rocket::IPNetAddr>("127.0.0.1",8001);  //这是一个IPNetAddr类指针
    DEBUGLOG("create addr %s", addr->toString().c_str());
    rocket::TcpServer tcp_server(addr);
    tcp_server.start();

}

int main(){
    rocket::Config::SetGlobalConfig("/home/wsh/code_rpc/tinyrpc/conf/tinyrpc.xml");
    rocket::Logger::InitGlobalLogger();
    test_tcp_server();




    // rocket::IPNetAddr addr("127.0.0.1",8001);
    // DEBUGLOG("create addr %s", addr.toString().c_str());
}