#pragma once
#ifndef __ZX_LOGEVENT_H__
#define __ZX_LOGEVENT_H__
#include<memory>
#include<iostream>
#include<sstream>
#include"LogLevel.h"
#include"../../util.h"

namespace zx{
class Logger;
class LogEvent {
public:
	typedef std::shared_ptr<LogEvent> ptr;
	LogEvent(std::shared_ptr<Logger>logger,LogLevel::Level level,const char* file, int32_t line, uint32_t elapse, uint32_t threadId, uint32_t fiberId, uint64_t time);
	const char* getFile()const { return m_file; }
	int32_t getLine() const{ return m_line; }//行号
	uint32_t getElapse()const { return m_elapse; }//程序启动开始到现在的毫秒数
	uint32_t getThreadId()const { return m_threadId; }//线程ID
	uint32_t getFiberId()const { return m_fiberId; }//协程ID
	uint64_t getTime()const { return m_time; }//时间戳
	std::string getComtent()const{return m_ss.str();}//内容
	std::stringstream& getSS() {
		return m_ss;
	}
	LogLevel::Level getLevel()const{
		return m_level;
	}
	std::shared_ptr<Logger> getLogger(){return m_logger;}
private:
	std::shared_ptr<Logger>m_logger;//日志器
	LogLevel::Level m_level;
	const char* m_file = nullptr;//文件名
	int32_t m_line = 0;//行号
	uint32_t m_elapse = 0;//程序启动开始到现在的毫秒数
	uint32_t m_threadId = 0;//线程ID
	uint32_t m_fiberId = 0;//协程ID
	uint64_t m_time;//时间戳
	std::stringstream m_ss;//数据

};
}
#endif