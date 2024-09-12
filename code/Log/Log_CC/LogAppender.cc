#include "../Log_H/LogAppender.h"
#include "../Log_H/Logger.h"
namespace zx{
    void FileLogAppender::log( LogLevel::Level level, LogEvent::ptr event) {
		if (level >= m_level) {
			std::string str= m_formatter->format( level, event);
			m_filestream << str;
		}
	}
	void StdoutLogAppender::log( LogLevel::Level level, LogEvent::ptr event) {
		if (level >= m_level) {
			std::string str = m_formatter->format( level, event);
			std::cout << str;
		}
	}
	 std::string StdoutLogAppender::toYamlString(){
		YAML::Node node;
		node["type"]="StdoutLogAppender";
		node["level"]=LogLevel::ToString(m_level);
		if(m_formatter){
			node["formatter"]=m_formatter->getPattern();
		}
		std::stringstream ss;
		ss<<node;
		return ss.str();
	 }
	 std::string FileLogAppender::toYamlString(){
		YAML::Node node;
		node["type"]="FileLogAppender";
		node["file"]=m_filename;
		node["level"]=LogLevel::ToString(m_level);
		if(m_formatter){
			node["formatter"]=m_formatter->getPattern();
		}
		std::stringstream ss;
		ss<<node;
		return ss.str();
	 }
	bool FileLogAppender::reopen() {
		if (m_filestream) {
			m_filestream.close();
		}
		m_filestream.open(m_filename);
		return !!m_filestream;
	}
	FileLogAppender::FileLogAppender(const std::string& filename) :m_filename(filename) {reopen();}
}