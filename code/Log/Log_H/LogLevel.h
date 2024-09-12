#pragma once
#ifndef __ZX_LOGLEVEL_H__
#define __ZX_LOGLEVEL_H__
#include <string>
namespace zx{
class LogLevel {
public:
	enum Level {
		UNNODE = 0,
		DEBUG = 1,
		INFO = 2,
		WARN = 3,
		ERROR = 4,
		FATAL = 5
	};
	static const char* ToString(LogLevel::Level level);
	static LogLevel::Level FromString(const std::string& v);
};
}
#endif
