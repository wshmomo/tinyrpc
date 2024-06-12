#ifndef CONFIG_H
#define CONFIG_H

#include<map>
#include<string>
#include<tinyxml/tinyxml.h>


using namespace std;

namespace rocket{

    class Config{
    public:

        Config(const char* xmlfile);
        static Config* GetGlobalConfig();

        static void SetGlobalConfig(const char* xmlfile);

    
        string m_log_level;

    }; 


};


#endif