#pragma once
#ifndef __ZX_LOGGER_H__
#define __ZX_LOGGER_H__
#include <memory>
#include <string>
#include "LogEvent.h"
#include <list>
#include "LogAppender.h"
#include "LogFormatter.h"
#include "LogEventWarn.h"
#define ZX_LOG_LEVEL(logger,level)\
    if(logger->getLevel()<=level)\
        zx::LogEventWarn(zx::LogEvent::ptr (new zx::LogEvent(logger,level,__FILE__,__LINE__,0,zx::GetThreadid(),zx::GetFiberId(),time(0)))).getSS()
#define  ZX_LOG_ROOT() zx::LoggerManger::LoggerMgr::GetInstance()->getRoot()
#define  ZX_LOG_DEBUGE(logger) ZX_LOG_LEVEL(logger,zx::LogLevel::DEBUG)
#define  ZX_LOG_INFO(logger) ZX_LOG_LEVEL(logger,zx::LogLevel::INFO)
#define  ZX_LOG_WARN(logger) ZX_LOG_LEVEL(logger,zx::LogLevel::WARN)
#define  ZX_LOG_ERROR(logger) ZX_LOG_LEVEL(logger,zx::LogLevel::ERROR)
#define  ZX_LOG_FATAL(logger) ZX_LOG_LEVEL(logger,zx::LogLevel::FATAL)
#define ZX_LOG_NAME(name) zx::LoggerManger::LoggerMgr::GetInstance()->getLogger(name)
namespace zx{
class Logger{ 
public:
	typedef std::shared_ptr<Logger> ptr;
	Logger(const std::string& name = "root");
	//打印日志
	void log(LogLevel::Level level,LogEvent::ptr event);
	void debug(LogEvent::ptr event);
	void info(LogEvent::ptr event);
	void warn(LogEvent::ptr event);
	void fatal(LogEvent::ptr event);
	void error(LogEvent::ptr event);
	//add or del appender
	void addAppender(LogAppender::ptr appender);
	void delAppender(LogAppender::ptr appender);
	void clearAppender();
	//set or get formatter
	void setFormatter(LogFormatter::ptr val);
	void setFormatter(const std::string&val);
	LogFormatter::ptr getFormatter();
	//set or get level
	LogLevel::Level getLevel()const { return m_level; }
	void setLevel(LogLevel::Level level) {m_level = level;}
	//getname
	const std::string getName()const {
		return m_name;
	}
	//setroot
	void setroot(Logger::ptr root){
		m_root=root;
	}
	std::string toYamlString();
private:
	std::string m_name;//日志名称
	LogLevel::Level m_level;//日志级别
	std::list<LogAppender::ptr>m_appenders;//列表，目的地集合
	LogFormatter::ptr m_formatter;//日志输出格式
	Logger::ptr m_root;//root的指针
};
}
#endif