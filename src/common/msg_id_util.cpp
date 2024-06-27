#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include "../../include/common/msg_id_util.h"
#include "../../include/common/log.h"



namespace rocket{
    static int g_msg_id_length = 20;
    static int g_random_fd =-1;
    static thread_local std::string t_msg_id_no;   //线程独立，当前线程的id
    static thread_local std::string t_max_msg_id_no;
    std::string MsgIDUtil::GetMsgID(){
        if(t_msg_id_no.empty() || t_msg_id_no == t_max_msg_id_no){
            if(g_random_fd == -1){
                g_random_fd = open("/dev/urandom", O_RDONLY);   //打开linux系统下的一个随机文件，读取整个文件的句柄
            }
            std::string res(g_msg_id_length,0);
            if((read(g_random_fd, &res[0], g_msg_id_length)) != g_msg_id_length){
                ERRORLOG("read from /dev/urandom error");
                return "";
            }
            for(int i = 0; i < g_msg_id_length; i++){
                uint8_t x = ((uint8_t)(res[i])) % 10;  //无符号整数
                res[i] = x + '0';
                t_max_msg_id_no += "9";
            }
            t_msg_id_no = res;  //这里的意思就是初始化为“00000000000000000000”

        }else{
            int i = t_msg_id_no.length()-1;
            while(t_msg_id_no[i] == '9' && i > 0){  
                i--;
            }//判断t_msg_id_no到达最大值
            if(i >= 0){
                t_msg_id_no[i] += 1;
                for(size_t j = i+1; j < t_msg_id_no.length(); j++){
                    t_msg_id_no[j] = '0';
                }
            }
        }
        return t_msg_id_no;
        

    }
}