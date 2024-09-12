#include <iostream>
#include <thread>
#include "../code/Log/Log_H/LoggerManger.h"
#include "../code/util.h"
#include <sstream>
int main(){
    auto i=zx::LoggerManger::LoggerMgr::GetInstance();
    auto logger1=i->getLogger("xxx");
    auto logger2=i->getLogger("zhangxuan");
    auto logger3=i->getLogger("liuxueqian");
    ZX_LOG_ERROR(logger1)<<"what happender";
     ZX_LOG_ERROR(logger2)<<"zhangxuan";
      ZX_LOG_ERROR(logger3)<<"liuxueqian";
    return 0;
}