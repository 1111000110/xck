#include "util.h"
namespace zx{
    pid_t GetThreadid(){
        return syscall(SYS_gettid);
    }
    uint32_t GetFiberId(){
        return 0;
    }
}