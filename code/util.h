#ifndef __ZX_UTIL_H__
#define __ZX_UTIL_H__
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdint.h>
#include <vector>
#include <string>

namespace zx{
    pid_t GetThreadid();//get线程id
    uint32_t GetFiberId();//get协程id
    void BackTrace(std::vector<std::string>&bt,int size=64,int skip=1);
   std::string BacktraceToString(int size=64,int skip=2,const std::string&prefix="");
    
}
#endif