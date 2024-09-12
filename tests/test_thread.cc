#include "../code/zx.h"
#include <vector>
#include <unistd.h>
#include "../code/Log/Log_H/LogDefine.h"
zx::Logger::ptr g_logger=ZX_LOG_ROOT();
int count=0;
zx::Mutex s_mutex;
/*void fun1(){
    ZX_LOG_INFO(g_logger)<<"name: "<<zx::Thread::GetName()
                                   <<"this.name :"<<zx::Thread::GetThis()->getName()
                                   <<"id :"<<zx::GetThreadid()
                                   <<"this.id :"<<zx::Thread::GetThis()->getId()
                                   <<"this.ip :"<<zx::Thread::GetThis();
    for(int i=0;i<100000;++i){
        zx::Mutex::Lock lock(s_mutex);
        ++count;
    }
}*/
void fun2(){
    int n=100;
    while(n--){
        ZX_LOG_INFO(ZX_LOG_NAME("zhangxuan"))<<"**************************************";
    }
}
void fun3(){
    int n=100;
        while(n--){
        ZX_LOG_INFO(ZX_LOG_NAME("zhangxuan"))<<"=======================================";
    }
}
int main(int argc,char **argv){
    ZX_LOG_INFO(g_logger)<<"thread test begin()";
    ZX_LOG_INFO(g_logger)<<zx::Thread::GetName();
    ZX_LOG_INFO(g_logger)<<zx::Thread::GetThis();
     std::cout<<zx::LoggerManger::LoggerMgr::GetInstance()->toYamlString()<<std::endl;
     YAML::Node root=YAML::LoadFile("log.yml");//加载yam格式的信息
     zx::Config::LoadFromYaml(root);
     std::cout<<"*****************************"<<std::endl;
    std::cout<<zx::LoggerManger::LoggerMgr::GetInstance()->toYamlString()<<std::endl;
    std::vector<zx::Thread::ptr>thrs;
    for(int i=0;i<2;++i){
        zx::Thread::ptr thr(new zx::Thread(&fun2,"name_"+std::to_string(i*2+1)));
        zx::Thread::ptr thr0(new zx::Thread(&fun3,"name_"+std::to_string(i*2)));
        thrs.push_back(thr);
        thrs.push_back(thr0);
    }
    for(int i=0;i<thrs.size();++i){
        thrs[i]->join();
    } ZX_LOG_INFO(g_logger)<<"thread test end()";
    ZX_LOG_INFO(g_logger)<<"count="<<count;
    return 0;
}