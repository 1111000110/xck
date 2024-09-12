
#include "../code/Config/Config_H/Config.h"
#include "../code/Log/Log_H/LoggerManger.h"
#include <yaml-cpp/yaml.h>
#include <vector>
#include <list>
#include <unordered_map>
#include "../code/Log/Log_H/LogDefine.h"
/*
zx::ConfigVar<std::list<int>>::ptr lptr=zx::Config::Lookup("system.list",std::list<int>{2,3},"system list");
zx::ConfigVar<int>::ptr g=zx::Config::Lookup("system.port",(int)8080,"system port");
zx::ConfigVar<float>::ptr gl=zx::Config::Lookup("system.port",(float)8080,"system port");
zx::ConfigVar<std::vector<int>>::ptr g2=zx::Config::Lookup("system.val",std::vector<int>(10,20),"system port");
zx::ConfigVar<std::set<int>>::ptr g3=zx::Config::Lookup("system.set",std::set<int>{10,20},"system port");
zx::ConfigVar<std::unordered_set<int>>::ptr g4=zx::Config::Lookup("system.uset",std::unordered_set<int>{10,20},"system port");
zx::ConfigVar<std::map<std::string,int>>::ptr g5=zx::Config::Lookup("system.map",std::map<std::string,int>{{"zx",6}},"system port");
zx::ConfigVar<std::unordered_map<std::string,int>>::ptr g6=zx::Config::Lookup("system.umap",std::unordered_map<std::string,int>{{"zx",6}},"system port");*/
void print_yaml(const YAML::Node&node,int level){
    if(node.IsScalar()){//标量
         ZX_LOG_INFO(ZX_LOG_ROOT())<<std::string(level*4,' ')<<node.Scalar()<<"-"<<node.Tag()<<"-"<<level;
    }
    else if(node.IsNull()){//空
        ZX_LOG_INFO(ZX_LOG_ROOT())<<std::string(level*4,' ')<<"NULL - "<<node.Tag()<<"-"<<level;
    }else if(node.IsMap()){//映射
        for(auto it=node.begin();it!=node.end();it++){
             ZX_LOG_INFO(ZX_LOG_ROOT())<<std::string(level*4,' ')<<it->first<<"-"<<it->second.Tag()<<"-"<<level;
            print_yaml(it->second,level+1);
        }
    }else if(node.IsSequence()){//序列
        for(size_t i=0;i<node.size();++i){
             ZX_LOG_INFO(ZX_LOG_ROOT())<<std::string(level*4,' ')<<i<<"-"<<node[i].Tag()<<"-"<<level;
            print_yaml(node[i],level+1);
        }
    }
}
void test_yaml(){
    YAML::Node root=YAML::LoadFile("log.yml");//加载yam格式的信息
    print_yaml(root,0);
    //ZX_LOG_INFO(ZX_LOG_ROOT())<<root;
}
class Person{
    public:
        std::string m_name=0;
        int m_age=0;
        bool m_sex=0;
        std::string toString()const{
            std::stringstream ss;
            ss<<"[Person name="<<m_name<<"age="<<m_age<<"sex="<<m_sex<<"]";
            return ss.str();
        }
};

void test_config(){
    #define XX(g2) {auto i0=g2->getValue();\
    for(auto &k:i0){\
        ZX_LOG_INFO(ZX_LOG_ROOT())<<k;\
    }}
    //XX(g4);
    #define XX_M(g2) {auto i0=g2->getValue();\
    for(auto &k:i0){\
        ZX_LOG_INFO(ZX_LOG_ROOT())<<k.first<<" "<<k.second;\
    }}
    #define CC(a) {\
        std::stringstream ss;\
        for(auto i:a){\
            ss<<i<<" ";\
        }\
         ZX_LOG_INFO(ZX_LOG_ROOT())<<"vale"<<ss.str();\
    }
    //XX(g4);
    //XX_M(g5);
     //XX(g2);
     //g2->addListender(10,[](const std::vector<int>&a,const std::vector<int>&b){
       // CC(a);
       // CC(b);
     //});
    // ZX_LOG_INFO(ZX_LOG_ROOT())<<zx::g_log_defines->toString();
    YAML::Node root=YAML::LoadFile("log.yml");//加载yam格式的信息
    zx::Config::LoadFromYaml(root);
    ZX_LOG_INFO(ZX_LOG_ROOT())<<"....................";
     ZX_LOG_INFO(ZX_LOG_ROOT())<<zx::g_log_defines->toString();
    //XX(g4);
    //XX_M(g5);
     //XX(g2);
     //ZX_LOG_INFO(ZX_LOG_ROOT())<<g->toString();

}
void test_logger(){
     std::cout<<zx::LoggerManger::LoggerMgr::GetInstance()->toYamlString()<<std::endl;
     YAML::Node root=YAML::LoadFile("log.yml");//加载yam格式的信息
     zx::Config::LoadFromYaml(root);
     std::cout<<"*******************************************"<<std::endl;
     std::cout<<zx::LoggerManger::LoggerMgr::GetInstance()->toYamlString()<<std::endl;
}
int main(){
    test_logger();
    //test_yaml();
    //test_config();
    //ZX_LOG_INFO(ZX_LOG_ROOT())<<g->getValue();
    //ZX_LOG_INFO(ZX_LOG_ROOT())<<g->toString();
    /*
        ZX_LOG_INFO(ZX_LOG_ROOT())<<"after:"<<g->getValue();
    ZX_LOG_INFO(ZX_LOG_ROOT())<<"after:"<<gl->getValue();
    XX(g2);
    XX(lptr);
    ZX_LOG_INFO(ZX_LOG_ROOT())<<g2->toString();
        ZX_LOG_INFO(ZX_LOG_ROOT())<<"before:"<<g->getValue();
    ZX_LOG_INFO(ZX_LOG_ROOT())<<"before:"<<gl->getValue();
    XX(g2);
    XX(lptr);
    */
    return 0;
}