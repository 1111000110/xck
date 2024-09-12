#include "../Log_H/LoggerManger.h"
namespace zx{
    LoggerManger::LoggerManger(){
        m_root.reset(new Logger("root"));
        m_root->addAppender(std::shared_ptr<LogAppender>(new zx::StdoutLogAppender));
        m_logger.insert({m_root->getName(),m_root});
    }
    Logger::ptr LoggerManger::getLogger(const std::string&name){
        auto it=m_logger.find(name);
        if(it!=m_logger.end()){
            return it->second;
        }
        Logger::ptr logger(new Logger(name));
        logger->setroot(m_root);
        m_logger.insert({name,logger});
        return logger;
    }
     std::string LoggerManger::toYamlString(){
        YAML::Node node;
        for(auto &i:m_logger){
            node.push_back(YAML::Load(i.second->toYamlString()));
        }
        std::stringstream ss;
        ss<<node;
        return ss.str();
     }
     bool  LoggerManger::add(Logger::ptr new_logger){
        m_logger.insert({new_logger->getName(),new_logger});
        return true;
     }
}