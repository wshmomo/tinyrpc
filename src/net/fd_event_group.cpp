#include "../../include/net/fd_event_group.h"

namespace rocket{

        static FDEventGroup* g_fd_event_group = NULL;
        FDEventGroup* FDEventGroup::getFDEventGroup(){
            if(g_fd_event_group != NULL){
                return g_fd_event_group;
            }
            g_fd_event_group = new FDEventGroup(128);   //这里的128，是初始化的大小，初始化多少后面可以进行修改
            return g_fd_event_group;

        }

        FDEventGroup::FDEventGroup(int size) : m_size(size){
            for(int i = 0; i < m_size; i++){  
                m_fd_group.push_back(new FDEvent(i));
            }

        }

        FDEventGroup::~FDEventGroup(){

            for(int i = 0; i < m_size; i++){
                if(m_fd_group[i] != NULL){
                    delete m_fd_group[i];
                    m_fd_group[i] = NULL;
                }
            }

        }
        
        //这里涉及到扩容的情况
        FDEvent* FDEventGroup::getFDEvent(int fd){
            lock_guard<mutex> lock(m_mutex);
            if(fd < m_fd_group.size()){
                return m_fd_group[fd];
            }

            int new_size = int (fd * 1.5);

            for(int i = m_fd_group.size(); i < new_size; i++){
                m_fd_group.push_back(new FDEvent(i));
            }
            return m_fd_group[fd];

        }

        
}