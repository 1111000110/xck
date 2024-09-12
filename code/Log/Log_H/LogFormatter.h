#pragma once
#ifndef __ZX_LOGFORMATTER_H__
#define __ZX_LOGFORMATTER_H__
#include <string>
#include <string.h>
#include <vector>
#include <tuple>
#include <memory>
#include <map>
#include "LogLevel.h"
#include "LogEvent.h"
#include <functional>
#include <time.h>
namespace zx{
//日志格式器
class Logger;
class LogFormatter {
public:
	typedef std::shared_ptr<LogFormatter>ptr;
	LogFormatter(const std::string& pattern);
	std::string format(LogLevel::Level level, LogEvent::ptr event);
public:
	class FormatItem {
	public:
		typedef std::shared_ptr<FormatItem>ptr;
		virtual ~FormatItem(){}
		virtual void format(std::ostream& os,  LogLevel::Level level, LogEvent::ptr event) = 0;
	};
	void init();
	bool isError()const{
		return m_error;
	}
	const std::string getPattern()const {return m_pattern;}
private:
	std::string m_pattern;
	std::vector<FormatItem::ptr>m_items;
	bool m_error=false;
};
class MessageFormatItem :public LogFormatter::FormatItem {
	public:
		MessageFormatItem(const std::string& str) {}
		void format(std::ostream& os,  LogLevel::Level level, LogEvent::ptr event) {
			os << event->getComtent();
		}

	};
	class LevelFormatItem :public LogFormatter::FormatItem {
	public:
		LevelFormatItem(const std::string& str) {}
		void format(std::ostream& os,  LogLevel::Level level, LogEvent::ptr event) {
			os << LogLevel::ToString(level);
		}

	};
	class ElapseFormatItem :public LogFormatter::FormatItem {
	public:
		ElapseFormatItem(const std::string& str) {}
		void format(std::ostream& os,  LogLevel::Level level, LogEvent::ptr event) {
			os << event->getElapse();
		}
	};
	class NameFormatItem :public LogFormatter::FormatItem {
	public:
		NameFormatItem(const std::string& str) {}
		void format(std::ostream& os,  LogLevel::Level level, LogEvent::ptr event)override;
	};
	class ThreadIdFormatItem :public LogFormatter::FormatItem {
	public:
		ThreadIdFormatItem(const std::string& str) {}
		void format(std::ostream& os,  LogLevel::Level level, LogEvent::ptr event) {
			os << event->getThreadId();
		}
	};
	class FiberIdFormatItem :public LogFormatter::FormatItem {
	public:
		FiberIdFormatItem(const std::string& str) {}
		void format(std::ostream& os,  LogLevel::Level level, LogEvent::ptr event) {
			os << event->getFiberId();
		}
	};
	class DateTimeFormatItem :public LogFormatter::FormatItem {
	public:
		DateTimeFormatItem(const std::string& format = "%Y-%m-%d %H:%M:%S") :m_format(format) {
            if(m_format.empty()){
                m_format="%Y-%m-%d %H:%M:%S";
            }
        };
		void format(std::ostream& os,  LogLevel::Level level, LogEvent::ptr event) {
			struct tm tma;//分解时间信息结构体
            time_t time=event->getTime();//获取时间戳
            localtime_r(&time,&tma);//本地时间转换
            char buf[64];
            strftime(buf,sizeof(buf),m_format.c_str(),&tma);//转化为指定格式字符串
            os<<buf;
		}
	private:
		std::string m_format;
	};
	class FilenameFormatItem :public LogFormatter::FormatItem {
	public:
		FilenameFormatItem(const std::string& format) {}
		void format(std::ostream& os,  LogLevel::Level level, LogEvent::ptr event) {
			os << event->getFile();
		}
	};
	class LineFormatItem :public LogFormatter::FormatItem {
	public:
		LineFormatItem(const std::string& str) {}
		void format(std::ostream& os,  LogLevel::Level level, LogEvent::ptr event) {
			os << event->getLine();
		}
	};
	class NewLineFormatItem :public LogFormatter::FormatItem {
	public:
		NewLineFormatItem(const std::string& str) {}
		void format(std::ostream& os, LogLevel::Level level, LogEvent::ptr event) {
			os << std::endl;
		}
	};
	class StringFormatItem :public LogFormatter::FormatItem {
	public:
		StringFormatItem(const std::string& str) : m_string(str) {}
		void format(std::ostream& os, LogLevel::Level level, LogEvent::ptr event) {
			os << m_string;
		}
	private:
		std::string m_string;
	};
    class TabFormatItem:public LogFormatter::FormatItem{
    public:
        TabFormatItem(const std::string& str){}
        void format(std::ostream& os,  LogLevel::Level level, LogEvent::ptr event) {
			os <<'\t';
		}
    };
}
#endif