#ifndef TINYPB_PROTOCOL_H
#define TINYPB_PROTOCOL_H
#include<string>
#include "abstract_protocol.h"


namespace rocket{
    class TinyPBProtocol : public AbstractProtocol{
    public:
         TinyPBProtocol(){}
        ~TinyPBProtocol(){}
        static char PB_START;

        static char PB_END;

    public:
        //定义协议的所有字段
        //整包的长度（4个字节）
        int32_t m_pk_len {0};

        int32_t m_msg_id_len {0};  //msg_id的长度
        //msgId继承父类这里指的是msg_id

        //方法名长度和方法名
        int32_t m_method_len {0};
        std::string m_method_name;

        //错误码，错误信息长度,错误信息
        int32_t m_err_code {0};
        int32_t m_err_info_len {0};
        std::string m_err_info;

        //protocol信息
        std::string m_pb_data;

        //校验和
        int32_t m_check_sum {0};

        bool m_parse_success {false};


        

    };
}


#endif