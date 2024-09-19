
#include "../Log_H/LogEventWarn.h"
#include "../Log_H/Logger.h"
#include <sstream>
namespace zx{
     LogEventWarn::LogEventWarn(LogEvent::ptr e):m_event(e){

     }
     LogEventWarn::~LogEventWarn(){
       m_event->getLogger()->log(m_event->getLevel(),m_event);
     }
     std::stringstream& LogEventWarn::getSS(){
         std::stringstream &ss=m_event->getSS();
        return m_event->getSS();
       
     }
}
