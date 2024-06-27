// #pragma once
#ifndef FDEVENTGROUP_H
#define FDEVENTGROUP_H


#include "fd_event.h"
#include "../common/log.h"

namespace rocket{
    class FDEventGroup{
        public:
            FDEventGroup(int size);
            ~FDEventGroup();

            FDEvent* getFDEvent(int fd);

            static FDEventGroup* getFDEventGroup();


        private:
            int m_size {0};
            vector<FDEvent*> m_fd_group;
            mutex m_mutex;


    };
}


#endif