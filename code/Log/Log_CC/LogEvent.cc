#include "../Log_H/LogEvent.h"

namespace zx{
  	LogEvent::LogEvent(std::shared_ptr<Logger>logger,LogLevel::Level level,const char* file, int32_t line, uint32_t elapse, uint32_t threadId, uint32_t fiberId, uint64_t time,const std::string& thread_name) :m_logger(logger),m_level(level),m_file(file),m_line(line),m_elapse(elapse),m_threadId(threadId),m_fiberId(fiberId),m_time(time),m_thread_name(thread_name){

	}
	
}

