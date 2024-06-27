#include<string.h>
#include "../../include/tcp/tcp_buffer.h"


namespace rocket{
    TcpBuffer::TcpBuffer(int size) : m_size(size){
        m_buffer.resize(size);

    }

    TcpBuffer::~TcpBuffer(){

    }


    //返回可读字节数
    int TcpBuffer::readAble(){
        return m_write_index - m_read_index;

    }


    //返回可写字节数
    int TcpBuffer::writeAble(){

        return m_buffer.size() - m_write_index;

    }

    //获取readIndex
    int TcpBuffer::readIndex(){
        return m_read_index;

    }

    int TcpBuffer::writeIndex(){
        return m_write_index;

    }
    
    //有个问题你在操作这些所有的东西的时候没有考虑锁
    //写入数据到Buffer中
    void TcpBuffer::wirteToBuffer(const char* buf, int size){
        if(size > writeAble()){
            //调整buffer的大小，进行扩容,这里的扩容这个字需要推敲一下
            int new_size = (int) (1.5 * (m_write_index + size));
            resizeBuffer(new_size);
            // DEBUGLOG("")
        }
        memcpy(&m_buffer[m_write_index], buf, size);
        m_write_index += size;

    }

     void TcpBuffer::resizeBuffer(int new_size){
        vector<char> tmp(new_size);
        int count = min(new_size,readAble());    //为什么选个最小的
        memcpy(&tmp[0], &m_buffer[m_read_index], count);   //感觉这块像是在去掉m_read_index前面的东西，因为count最大就是readAble(),也就是m_buffer.size() - m_write_index
        m_buffer.swap(tmp);
        m_read_index = 0;
        m_write_index = m_read_index + count;


     }

    //从Buffer中读取数据
    void TcpBuffer::readFromBuffer(vector<char>& re,int size){
        if(readAble() == 0){
            return;
        }

        int read_size = readAble() > size? size: readAble();
        vector<char> tmp(read_size);
        memcpy(&tmp[0], &m_buffer[m_read_index], read_size);
        re.swap(tmp);
        m_read_index += read_size;
        adjustBuffer();

    }

    void TcpBuffer::adjustBuffer(){
        //给一个调整数，若大于整个buffer的1/3就需要调整
        if(m_read_index < int(m_buffer.size() /3)){
            return ;
        }
        
        //注意这里的调整和上面的扩容不一样，这里的容量没有发生变化
        vector<char> buffer(m_buffer.size());

        int count = readAble();

        memcpy(&buffer[0], &m_buffer[m_read_index],count);
        m_buffer.swap(buffer);
        m_read_index = 0;
        m_write_index = m_read_index + count;

        buffer.clear();

    }


    //调整m_read_index调整，说明这块已经读了
    void TcpBuffer::moveReadIndex(int size){
        int j = m_read_index + size;
        if( j >= m_buffer.size()){
            ERRORLOG("moveReadIndex error, invalid size %d, old_read_index %d, buffer size %d", size,m_read_index, m_buffer.size());
            return;

        }
        m_read_index = j;
        adjustBuffer();

    }

    //调整m_write_index
    void TcpBuffer::moveWriteIndex(int size){
        int j = m_write_index + size;
        if( j >= m_buffer.size()){
            ERRORLOG("moveWriteIndex error, invalid size %d, old_write_index %d, buffer size %d", size,m_read_index, m_buffer.size());
            return;

        }
        m_write_index = j;
    }
}