#ifndef __ZX_LOGGERMANGER_H__
#define __ZX_LOGGERMANGER_H__
#include "../../singleton.h"
#include "Logger.h"

namespace zx{
class LoggerManger{
public:
    typedef SingletonPtr<LoggerManger> LoggerMgr;
    LoggerManger();
    Logger::ptr getLogger(const std::string&name);
    Logger::ptr getRoot()const{return m_root;}
    std::string toYamlString();
    bool add(Logger::ptr new_logger);
    int sum(){
        return m_logger.size();
    }
    void print(){
        for(auto i:m_logger){
            std::cout<<i.first<<std::endl;
        }
    }
private:
    std::map<std::string,Logger::ptr>m_logger;
    Logger::ptr m_root;
};
}
#endif