
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




void test_connect(){

    //1.
    int fd = socket(AF_INET,SOCK_STREAM,0);

    if(fd < 0){
        ERRORLOG("invalid fd %d", fd);
        exit(0);
    }

    sockaddr_in server_addr;
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8001);
    inet_aton("127.0.0.1",&server_addr.sin_addr);

    int rt = connect(fd, (sockaddr*)&server_addr,sizeof(server_addr));
    
    string msg = "hello";

    rt = write(fd, msg.c_str(),msg.length());
    DEBUGLOG("success write %d bytes, [%s]", rt, msg.c_str());

    char buf[100];
    rt = read(fd,buf,100);
    DEBUGLOG("success read %d bytes, [%s]", rt, string(buf).c_str());
    





}

void test_tcp_client(){
    rocket::IPNetAddr::s_ptr addr = make_shared<rocket::IPNetAddr>("127.0.0.1",8001); 
    rocket::TcpClient client(addr);
    client.connect([&](){
        DEBUGLOG("connect to [%s] success", addr->toString().c_str());
        // shared_ptr<rocket::StringProtocol> message = make_shared<rocket::StringProtocol>();
        // message->info = "hello rocket";
        // message->m_msg_id = "123456";
        // //先发送消息，然后在等其对应的回包，进行读取
        // client.writeMessage(message, [](rocket::AbstractProtocol::s_ptr msg_ptr){
        //     DEBUGLOG("send message success");

        // });

        // client.readMessage("123456", [](rocket::AbstractProtocol::s_ptr msg_ptr){
        //     shared_ptr<rocket::StringProtocol> message = dynamic_pointer_cast<rocket::StringProtocol> (msg_ptr); //将父类的智能指针转化为子类的智能指针
        //     DEBUGLOG("read message success, req_if[%s], get response %s",message->m_msg_id.c_str(), message->info.c_str());

        // });

        // client.writeMessage(message, [](rocket::AbstractProtocol::s_ptr msg_ptr){
        //     DEBUGLOG("send message 2222 success");

        // });

        shared_ptr<rocket::TinyPBProtocol> message = make_shared<rocket::TinyPBProtocol>();
        
        message->m_msg_id = "123456789";
        message->m_pb_data = "test pb data";
        //先发送消息，然后在等其对应的回包，进行读取
        client.writeMessage(message, [](rocket::AbstractProtocol::s_ptr msg_ptr){
            DEBUGLOG("send message success");

        });

        client.readMessage("123456789", [](rocket::AbstractProtocol::s_ptr msg_ptr){
            shared_ptr<rocket::TinyPBProtocol> message = dynamic_pointer_cast<rocket::TinyPBProtocol> (msg_ptr); //将父类的智能指针转化为子类的智能指针
            DEBUGLOG("read message success, req_if[%s], get response %s",message->m_msg_id.c_str(), message->m_pb_data.c_str());

        });
    });


}

int main(){
    rocket::Config::SetGlobalConfig("/home/wsh/code_rpc/tinyrpc/conf/tinyrpc.xml");
    rocket::Logger::InitGlobalLogger();
    // test_connect();
    test_tcp_client();

    return 0;
    




    // rocket::IPNetAddr addr("127.0.0.1",8001);
    // DEBUGLOG("create addr %s", addr.toString().c_str());
}