#include"config.h"



#define READ_XML_NODE(name,parent)\
    TiXmlElement* name##_node = parent->FirstChildElement(#name);\  
    if(!name##_node){                                   \  
        printf("Start rocket server error, failed to read node [%s]", #name); \
        exit(0);  \
    } \

#define READ_STR_FROM_XML_NODE(name,parent)\
    TiXmlElement* name##_node = parent->FirstChildElement(#name);\  
    if(!name##_node ||!name##_node->GetText()){                                   \  
        printf("Start rocket server error, failed to read node [%s]", #name); \
        exit(0);  \
    } \
    string name##_str = string(name##_node->GetText());\

//注：宏定义中 x##y表述字符串x和字符串y拼接，x#@,表示对x加单引号，即'x'，#x表示对x加双引号,即"x"
namespace rocket{


    static Config* g_config = nullptr;

    Config* Config::GetGlobalConfig(){
        return g_config;

    }

    void Config::SetGlobalConfig(const char* xmlfile){
        if(g_config == nullptr){
            g_config = new Config(xmlfile);
        }
        

    }


    Config:: Config(const char* xmlfile){
        TiXmlDocument* xml_document = new TiXmlDocument();
        bool rt = xml_document->LoadFile(xmlfile);
        if(!rt){    //这里表示上述的xmfile文件加载失败
            printf("Start rocket server error, failed to read config file %s, error info[%s]",xmlfile,xml_document->ErrorDesc());
            exit(0);
        }

        //接下来需要遍历节点
        //第一个根节点
        // TiXmElement* root_node = xml_document->FirstChildElement("root");
        // if(!root_node){  //等于空时
        //     printf("Start rocket server error, failed to read node [%s]","root");
        //     exit(0);
        // }
        
        // //第二个子节点，由于这两是一样的，所以这里考虑用一个函数表示下
        // TiXmElement* log_node = root_node->FirstChildElement("log");
        // if(!log_node){  //等于空时
        //     printf("Start rocket server error, failed to read node [%s]","log");
        //     exit(0);
        // }


        READ_XML_NODE(root,xml_document);   //等同于上面那个
        READ_XML_NODE(log,root_node);

        // TiXmElement* log_level_node = log_node->FirstChildElement("log_level");
        // if(!log_level_node || !log_level_node->GetText()){
        //     printf("Start rocket server error, failed to read node [%s]","log_level")
        // }

        // string log_level = string(log_level_node->GetText());
        READ_STR_FROM_XML_NODE(log_level,log_node);

        m_log_level = log_level_str;


        
        

    }
}