#ifndef __ZX_UTIL_H__
#define __ZX_UTIL_H__
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdint.h>
#include <vector>
namespace zx{
    pid_t GetThreadid();//get线程id
    uint32_t GetFiberId();//get协程id

}
#endif