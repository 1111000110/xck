#include "../Log_H/Logger.h"
namespace zx{
    Logger::Logger(const std::string& name):m_name(name),m_level(LogLevel::DEBUG){
            m_formatter.reset(new LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]  %f:%l  %T%m %n"));
		}
    void Logger::log(LogLevel::Level level, LogEvent::ptr event) {
        if (level >= m_level) {
			if(!m_appenders.empty()){
				for (auto i : m_appenders) {
                i->log(level, event);
            	}
			}
			else if(m_root){
				m_root->log(level,event);
			}
        }
    }
	void Logger::setFormatter(LogFormatter::ptr val){
		m_formatter=val;
		for(auto &i:m_appenders){
			if(!i->get_formatter_is_true_false()){
				i->setFormatter(val);
			}
		}
	}
	void Logger::setFormatter(const std::string&val){
		zx::LogFormatter::ptr new_val(new zx::LogFormatter(val));
		if(new_val->isError()){
			std::cout<<"Logger setFormatter name="<<m_name<<"valeu="<<val<<"invalid formatter"<<std::endl;
			return ;
		}
		setFormatter(new_val);
	}
	LogFormatter::ptr Logger::getFormatter(){
		return m_formatter;
	}
	std::string Logger::toYamlString(){
		YAML::Node node;
		node["name"]=m_name;
		node["level"]=LogLevel::ToString(m_level);
		if(m_formatter){
			node["formatter"]=m_formatter->getPattern();
		}
		for(auto &i:m_appenders){
			node["appenders"].push_back(YAML::Load(i->toYamlString()));
		}
		std::stringstream ss;
		ss<<node;
		return ss.str();
		
	}
	void Logger::clearAppender(){
		m_appenders.clear();
	}
    void Logger::debug(LogEvent::ptr event) {
		log(LogLevel::DEBUG, event);
	}
	void Logger::info(LogEvent::ptr event) {
		log(LogLevel::INFO, event);
	}
	void Logger::warn(LogEvent::ptr event) {
		log(LogLevel::WARN, event);
	}
	void Logger::fatal(LogEvent::ptr event) {
		log(LogLevel::FATAL, event);
	}
	void Logger::error(LogEvent::ptr event) {
		log(LogLevel::ERROR, event);
	}
	void Logger::addAppender(LogAppender::ptr appender){
		if (!appender->getFormatter()) {
			appender->setFormatter(m_formatter);
		}
		else{
			appender->set_formatter_is_true();
		}
		m_appenders.push_back(appender);
	}
	void Logger::delAppender(LogAppender::ptr appender){
		for (auto i = m_appenders.begin(); i != m_appenders.end(); i++) {
			if (*i == appender) {
				m_appenders.erase(i);
				break;
			}
		}
	}
}