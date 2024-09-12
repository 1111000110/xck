#ifndef __ZX_LOG_LOG_G_LOGDEFINE_H__
#define __ZX_LOG_LOG_G_LOGDEFINE_H__
#include <string>
#include "Logger.h"
#include "../../Config/Config_H/Config.h"
namespace zx{
    struct LogAppenderDefine{
        int type=0;//1 File 2 Stdout
       LogLevel::Level level;
       std::string formattter;
       std::string file;
       bool operator==(const LogAppenderDefine&oth)const{
            return type==oth.type
            &&level==oth.level
            &&formattter==oth.formattter
            &&file==oth.file;
       }
    };
    struct LogDefine{
        std::string name;
        LogLevel::Level level;
        std::string formatter;
        std::vector<LogAppenderDefine>appenders;
        bool operator==(const LogDefine&oth)const{
            return name==oth.name
            &&level==oth.level
            &&formatter==oth.formatter
            &&appenders==oth.appenders;
        }
        bool operator<(const LogDefine&oth)const{
            return name<oth.name;
        }
       
    };
    template<>
    class LexicalCast<std::string, std::set<LogDefine>>{
        public:
       std::set<LogDefine> operator()(const std::string &v){
            YAML::Node n=YAML::Load(v);
            std::set<LogDefine> anser;
            for(auto node:n){
                LogDefine id;
                if(!node["name"].IsDefined()){
                    std::cout<<"Logconfig error :name is null "<<node<<std::endl;
                    continue ;
                }
                id.name=node["name"].as<std::string>();
                id.level=LogLevel::FromString(node["level"].IsDefined()?node["level"].as<std::string>():"");
                if(node["formatter"].IsDefined()){
                    id.formatter=node["formatter"].as<std::string>();
                }
                if(node["appenders"].IsDefined()){
                    for(size_t n=0;n<node["appenders"].size();n++){
                        auto a=node["appenders"][n];
                        if(!a["type"].IsDefined()){
                            std::cout<<"Logconfig error :appender type is null "<<a<<std::endl;
                            continue;
                        }
                        std::string type=a["type"].as<std::string>();
                        LogAppenderDefine lad;
                        if(a["formatter"].IsDefined()){
                            lad.formattter=a["formatter"].as<std::string>();
                        }
                        if(a["level"].IsDefined()){
                            lad.level=LogLevel::FromString(a["level"].as<std::string>());
                        }
                        else{
                            lad.level=LogLevel::FromString("INFO");
                        }
                        if(type=="FileLogAppender"){
                            lad.type=1;
                            if(!a["file"].IsDefined()){
                                std::cout<<"Logconfig error :appender type is FileLogAppender but file is not exits"<<a<<std::endl;
                                continue;
                            }
                            lad.file=a["file"].as<std::string>();
                            
                        }else if(type=="StdoutLogAppender"){
                            lad.type=2;
                        }else {
                            std::cout<<"Logconfig error :appender type is invalid "<<a<<std::endl;
                            continue;
                        }
                        id.appenders.push_back(lad);
                    }

                }
                anser.insert(id);
            }
            return anser;
       }   
    };
    template<>
    class LexicalCast<std::set<LogDefine>,std::string>{
        public:
        std::string operator()(const std::set<LogDefine>&v){
            YAML::Node node;
            for(auto &i:v){
                YAML::Node n;
                n["name"]=i.name;
                if(i.level==LogLevel::UNNODE){
                    n["level"]=LogLevel::ToString(i.level);
                }
                if(i.formatter.empty()){
                    n["formatter"]=i.formatter;
                }
                for(auto &k:i.appenders){
                    YAML::Node na;
                    if(k.type==1){
                        na["type"]="FileLogAppender";
                        na["file"]=k.file;
                    }
                    else if(k.type==2){
                        na["type"]="StdoutLogAppender";
                    }

                    na["level"]=LogLevel::ToString(k.level);
                    if(!k.formattter.empty()){
                        na["formatter"]=k.formattter;
                    }
                    n["appenders"].push_back(na);
                }
                node.push_back(n);
            }
            std::stringstream ss;
            ss<<node;
            return ss.str();
        }
    };
     zx::ConfigVar<std::set<LogDefine>>::ptr g_log_defines=zx::Config::Lookup("logs",std::set<LogDefine>(),"log config");
    struct LogIniter{
         LogIniter(){
            g_log_defines->addListender(0xF1E231,[](const std::set<LogDefine>&old_value,const std::set<LogDefine>&new_value){
                //新增
                ZX_LOG_INFO(ZX_LOG_ROOT())<<"on_logger_conf_changed";
                for(auto &i:new_value){
                    auto it=old_value.find(i);
                     zx::Logger::ptr logger;
                    if(it==old_value.end()){
                       logger.reset(new zx::Logger(i.name));
                    }
                    else{
                        if(!(i==*it)){
                            //修改logger
                            logger=ZX_LOG_NAME(i.name);
                        }
                    }
                    logger->setLevel(i.level);
                    if(!i.formatter.empty()){
                        logger->setFormatter(i.formatter);
                    }
                    //新增logger
                    logger->clearAppender();
                    for(auto &a:i.appenders){
                        zx::LogAppender::ptr ap;
                        if(a.type==1){
                            ap.reset(new FileLogAppender(a.file));
                        }else if(a.type==2){
                            ap.reset(new StdoutLogAppender());
                        }
                        if(!a.formattter.empty())ap->setFormatter(LogFormatter::ptr (new LogFormatter(a.formattter)));
                        ap->setLevel(a.level);
                        logger->addAppender(ap);
                    }
                    zx::LoggerManger::LoggerMgr::GetInstance()->add(logger);
                    
                }
                //删除
                for(auto&i:old_value){
                    auto it=new_value.find(i);
                    if(it==new_value.end()){
                        //删除logger
                        auto logger=ZX_LOG_NAME(i.name);
                        logger->setLevel((LogLevel::Level)100);
                        logger->clearAppender();
                    }
                }
            });
         }
    };
    
    
    static LogIniter __log_init;
}
#endif