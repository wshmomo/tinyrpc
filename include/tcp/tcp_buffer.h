#pragma once
#ifndef TCPBUFFFER_H
#define TCPBUFFFER_H
#include<vector>
#include<memory>
#include"../common/log.h"

namespace rocket{
    class TcpBuffer{
    public:
        typedef std::shared_ptr<TcpBuffer> s_ptr;
        TcpBuffer(int size);
        ~TcpBuffer();


        //返回可读字节数
        int readAble();


        //返回可写字节数
        int writeAble();

        //获取readIndex
        int readIndex();

        int writeIndex();
        
        //写入数据到Buffer中
        void wirteToBuffer(const char* buf, int size);

        //从Buffer中读取数据
        void readFromBuffer(vector<char>& re,int size);

        //扩容函数
        void resizeBuffer(int nwe_size);
        

        //调整整个m_buffer的内容，这里不扩容，只是清除已经读过的内容，然后进行平移
        void adjustBuffer();
        
        //调整m_read_index调整，说明这块已经读了
        void moveReadIndex(int size);  

        //调整m_write_index
        void moveWriteIndex(int size); 


    private:
        int m_read_index {0};
        int m_write_index {0};
        int m_size {0}; //buffer的容量大小
        
    public:
        vector<char> m_buffer = {0};

    };
}


#endif 