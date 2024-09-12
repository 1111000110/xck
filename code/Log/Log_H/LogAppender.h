#pragma once
#ifndef __ZX_LOGAPPENDER_H__
#define __ZX_LOGAPPENDER_H__
#include <memory>
#include <fstream>
#include "LogLevel.h"
#include "LogFormatter.h" 
//#include "../../Thread/Thread_H/RWMutex.h"
#include <yaml-cpp/yaml.h>
namespace zx{

//日志输出地
class Logger;
class LogAppender {
public:
	//LogAppender(){}
	typedef std::shared_ptr<LogAppender> ptr;
	virtual ~LogAppender(){}
	virtual void log(LogLevel::Level level, LogEvent::ptr event) = 0;
	void setFormatter(LogFormatter::ptr val) {
		m_formatter = val;
	}
	LogFormatter::ptr getFormatter()const {
		return m_formatter;
	}
	void setLevel(LogLevel::Level level){
		m_level=level;
	}
	void set_formatter_is_true(){m_set_formatter=true;}
	void set_formatter_is_false(){m_set_formatter=false;}
	bool get_formatter_is_true_false(){return m_set_formatter;}
	virtual std::string toYamlString()=0;
protected:
	LogLevel::Level m_level=LogLevel::UNNODE;
	bool m_set_formatter=false;//是否自己有自己的formatter

	LogFormatter::ptr m_formatter;
};

//定义输出到stdout
class StdoutLogAppender :public LogAppender {
public:
	typedef std::shared_ptr<LogAppender> ptr;
	virtual void log( LogLevel::Level level, LogEvent::ptr event) override;
	virtual std::string toYamlString()override;
private:

};
//定义输出到文件
class FileLogAppender :public LogAppender {
public:
	FileLogAppender(const std::string& filename);
	typedef std::shared_ptr<FileLogAppender> ptr;
	virtual void log(LogLevel::Level level, LogEvent::ptr event)override;
	//重新打开文件，文件打开成功返回true
	bool reopen();
	virtual std::string toYamlString()override;
private:
	std::string m_filename;
	std::ofstream m_filestream;
};
}
#endif