#include "../Log_H/LogLevel.h"
namespace zx{
   const char* LogLevel::ToString(LogLevel::Level level){
		 switch (level) {
			#define XX(name) case LogLevel::name: return #name; break;
			 XX(DEBUG);
			 XX(INFO);
			 XX(WARN);
			 XX(ERROR);
			 XX(FATAL);
			#undef XX
		 default :
			 return "UNKNOW";
		 }
		 return "UNKNOW";
   }
   LogLevel::Level LogLevel::FromString(const std::string& v){
			#define XX(level, str) \
			if(v == #str) { \
				return LogLevel::level; \
			}
			XX(DEBUG, debug);
			XX(INFO, info);
			XX(WARN, warn);
			XX(ERROR, error);
			XX(FATAL, fatal);

			XX(DEBUG, DEBUG);
			XX(INFO, INFO);
			XX(WARN, WARN);
			XX(ERROR, ERROR);
			XX(FATAL, FATAL);
			return LogLevel::UNNODE;
			#undef XX
   }
}
