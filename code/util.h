#ifndef __ZX_UTIL_H__
#define __ZX_UTIL_H__
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdint.h>
namespace zx{
    pid_t GetThreadid();
    uint32_t GetFiberId();
}
#endif