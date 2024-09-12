#ifndef __ZX_LOGEVENTWATN_H__
#define __ZX_LOGEVENTWATN_H__
#include "LogEvent.h"
#include <sstream>
namespace zx{
class LogEventWarn{
    public:
     LogEventWarn(LogEvent::ptr e);
     ~LogEventWarn();
     std::stringstream& getSS();
    private:
    LogEvent::ptr m_event;
};
}
#endif