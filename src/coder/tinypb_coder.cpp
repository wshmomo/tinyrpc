#include<arpa/inet.h>
#include "../../include/coder/tinypb_coder.h"
#include "../../include/coder/tinypb_protocol.h"
#include "../../include/common/util.h"

namespace rocket{
    
    
    //编码，将message对象转化为字节流，写入到buffer
    void TinyPBCoder::encode(std::vector<AbstractProtocol::s_ptr>& messages, TcpBuffer::s_ptr out_buffer){
        for(auto &i : messages){
            std::shared_ptr<TinyPBProtocol> msg = std::dynamic_pointer_cast<TinyPBProtocol>(i);
            int len =0;
            const char* buf = encodeTinyPB(msg, len);
            if(buf != NULL && len != 0){
                out_buffer->wirteToBuffer(buf,len);
            }
            if(buf){  //因为这里是malloc动态申请的，所以需要释放掉,防止内存泄漏
                free((void*)buf);
                buf = NULL;
            }
        }

    }


    //解码，将buffer里面的字节流转化为message对象
    void TinyPBCoder::decode(std::vector<AbstractProtocol::s_ptr>& out_messages, TcpBuffer::s_ptr buffer){

        while(1){
            //从这里开始到下面只解析了一个包,所以需要一个循环解析多个包
            //遍历buffer, 找到PB_START,找到之后，解析出整包长度，由PB_START和整包长度就可以找到结束符位置，判断其是否为PB_END。
            std::vector<char> tmp = buffer->m_buffer;
            int start_index = buffer->readIndex();
            int end_index = -1;
            int pk_len = 0;
            bool parse_sucess =false;
            int i= 0;
            
            for(i =start_index; i < buffer->writeIndex(); i++){
                if(tmp[i] == TinyPBProtocol::PB_START){
                    //读下去四个字节，由于是网络字节序(大端），所以需要转为主机字节序
                    if(i + 1 < buffer->writeIndex()){
                        pk_len = getInt32FromNetByte(&tmp[i+1]);
                        DEBUGLOG("get pk_len = %d",pk_len);

                        int j = i + pk_len -1; //结束符索引
                        if( j >= buffer->writeIndex()){//说明没有读到整个包,跳出这个循环
                            continue;

                        }
                        if(tmp[j] == TinyPBProtocol::PB_END){
                            start_index = i;
                            end_index = j;
                            parse_sucess = true;  //说明读成功了，跳出循环
                            break;
                        }
                    }
                }
            }

            if(i >= buffer->writeIndex()){//说明包已经读完了
                DEBUGLOG("decode end, read all buffer data");
                return;
            }

            if(parse_sucess){
                buffer->moveReadIndex(end_index - start_index + 1);  //送入的参数是读出的字节数
                std::shared_ptr<TinyPBProtocol> message = std::make_shared<TinyPBProtocol>();  //这里我们应该使用指向子类的只能指针，所以TinyPBProtocol::s_ptr message = std::make_shared<TinyPBProtocol>(); 这样写是错误的
                message->m_pk_len = pk_len;

                int req_id_len_index = start_index + sizeof(char) + sizeof(message->m_pk_len);
                if(req_id_len_index >= end_index){
                    message->m_parse_success = false;
                    ERRORLOG("parse error, reg_id_len_index[%d] >= end_index[%d]", req_id_len_index, end_index);
                    continue;
                }
                message->m_req_id_len = getInt32FromNetByte(&tmp[req_id_len_index]);
                DEBUGLOG("parse req_id_len = %d", message->m_req_id_len);
                int req_id_index = req_id_len_index + sizeof(message->m_req_id_len);
                char req_id[100] ={0};
                memcpy(&req_id[0], &tmp[req_id_index], message->m_req_id_len);
                message->m_req_id = std::string(req_id);
                DEBUGLOG("parse req_id=%s", message->m_req_id.c_str());

                int method_name_len_index = req_id_index + message->m_req_id_len;
                if(method_name_len_index >= end_index){
                    message->m_parse_success = false;
                    ERRORLOG("parse error, method_name_len_index[%d] >= end_index[%d]", method_name_len_index, end_index);
                    continue;
                }

                message->m_method_len = getInt32FromNetByte(&tmp[method_name_len_index]);

                int method_name_index = method_name_len_index + sizeof(message->m_method_len);
                char method_name[100] ={0};
                memcpy(&method_name[0], &tmp[method_name_index], message->m_method_len);
                message->m_method_name = std::string(method_name);
                DEBUGLOG("parse menthd_name = %s", message->m_method_name.c_str());

                int err_code_index = method_name_index + message->m_method_len;
                if(err_code_index >= end_index){
                    message->m_parse_success = false;
                    ERRORLOG("parse error, err_code_index[%d] >= end_index[%d]", err_code_index, end_index);
                    continue;
                }
                message->m_err_code = getInt32FromNetByte(&tmp[err_code_index]);

                int error_info_len_index = err_code_index + sizeof(message->m_err_code);
                if(error_info_len_index >= end_index){
                    message->m_parse_success = false;
                    ERRORLOG("parse error, error_info_len_index[%d] >= end_index[%d]", error_info_len_index, end_index);
                    continue;
                }
                message->m_err_info_len = getInt32FromNetByte(&tmp[error_info_len_index]);

                int error_info_index = error_info_len_index + sizeof(message->m_err_info_len);
                char error_info[100] ={0};
                memcpy(&error_info[0], &tmp[error_info_index], message->m_err_info_len);
                message->m_err_info = std::string(error_info);
                DEBUGLOG("parse error_info=%s", message->m_err_info.c_str());

                //序列化的长度需要用其他的进行减
                int pb_data_len = message->m_pk_len - message->m_method_len - message->m_err_info_len - 2 - 24;

                int pb_data_index = error_info_index + message->m_err_info_len;
                message->m_pb_data = std::string(&tmp[pb_data_index],pb_data_index); //同上面的拷贝差不多的方法

                //这里的校验算法这里并不是很重要，如果想要研究

                message->m_parse_success = true;

                out_messages.push_back(message);
            }

        }

    }
    
    const char* TinyPBCoder::encodeTinyPB(std::shared_ptr<TinyPBProtocol> message, int& len){
        if(message->m_req_id.empty()){
            message->m_req_id = "123456789";
        }

        DEBUGLOG("req_id = %s", message->m_req_id.c_str());
        int pk_len = 2 + 24 + message->m_req_id.length() + message->m_method_name.length() + message->m_err_info.length() + message->m_pb_data.length();  //为什么这里不直接使用m_err_info_len之类的，为了避免里面是空
        DEBUGLOG("pk_len = %d", pk_len);
        char* buf = (char*) malloc(pk_len);  //申请一段动态内存
        char* tmp = buf;
        *tmp = TinyPBProtocol::PB_START;
        tmp++;
        int32_t pk_len_net = htonl(pk_len);
        memcpy(tmp, &pk_len_net, sizeof(pk_len_net));
        tmp += sizeof(pk_len_net);

        int req_id_len = message->m_req_id.length();
        int32_t req_id_len_net = htonl(req_id_len);
        memcpy(tmp, &req_id_len_net, sizeof(req_id_len_net));
        tmp += sizeof(req_id_len_net);

        if(!message->m_req_id.empty()){
            memcpy(tmp, &(message->m_req_id[0]), req_id_len);
            tmp += req_id_len;
        }

        int method_name_len = message->m_method_name.length();
        int32_t method_name_len_net = htonl(method_name_len);
        memcpy(tmp, &method_name_len_net,sizeof(method_name_len_net));
        tmp += sizeof(method_name_len_net);

        if(!message->m_method_name.empty()){
            memcpy(tmp, &(message->m_method_name[0]), method_name_len);
            tmp += method_name_len;
        }

        int32_t err_code_net = htonl(message->m_err_code);
        memcpy(tmp, &err_code_net,sizeof(err_code_net));
        tmp += sizeof(err_code_net);

        int err_info_len = message->m_err_info.length();
        int32_t err_info_len_net = htonl(err_info_len);
        memcpy(tmp, &err_info_len_net,sizeof(err_info_len_net));
        tmp += sizeof(err_info_len_net);

        if(!message->m_err_info.empty()){
            memcpy(tmp, &(message->m_err_info[0]), err_info_len);
            tmp += err_info_len;
        }

        if(!message->m_pb_data.empty()){
            memcpy(tmp, &(message->m_pb_data[0]), message->m_pb_data.length());
            tmp += message->m_pb_data.length();
        }

        int32_t check_sum_net = htonl(1);
        memcpy(tmp,&check_sum_net,sizeof(check_sum_net));
        tmp += sizeof(check_sum_net);

        *tmp = TinyPBProtocol::PB_END;

        message->m_pk_len = pk_len;
        message->m_req_id_len = req_id_len;
        message->m_method_len = method_name_len;
        message->m_err_info_len = err_info_len;
        message->m_parse_success = true;
        len = pk_len;



        DEBUGLOG("encode message[%s] success", message->m_req_id.c_str());
        return buf;




        


    }

}