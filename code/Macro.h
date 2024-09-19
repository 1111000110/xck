#ifndef __ZX_MACOR_H__
#define __ZX_MACOR_H__
#include <string.h>
#include <assert.h>
#include "util.h"
#define ZX_ASSERT(x)\
    if(!(x)){\
        ZX_LOG_ERROR(ZX_LOG_ROOT())<<"ASSERTION: "<<#x\
            <<"\nbacktrace\n"\
            <<zx::BacktraceToString(100,2,"    ");\
            assert(x);\
    }
#define ZX_ASSERT2(x,w)\
    if(!(x)){\
        ZX_LOG_ERROR(ZX_LOG_ROOT())<<"ASSERTION: "<<#x\
            <<"\n"<<w\
            <<"\nbacktrace\n"\
            <<zx::BacktraceToString(100,2,"    ");\
            assert(x);\
    }

#endif