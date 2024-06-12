#ifndef IOTHREADGROUP_H
#define IOTHREADGROUP_H

#include <vector>
#include "log.h"
#include "io_thread.h"


namespace rocket{
    //IO线程池
    class IOThreadGroup{
    public:
        IOThreadGroup(int size);
        ~IOThreadGroup();
        void start();

        void join();

        IOThread* getIOThread();

    private:
        int m_size {0};
        vector<IOThread*> m_io_thread_groups;

        int m_index {0}; // 当前轮询的IOThread的标识


    };
}



#endif