#include "util.h"
#include <execinfo.h>
#include "Log/Log_H/LoggerManger.h"
#include "Fiber/Fiber_H/fiber.h"
namespace zx{
    zx::Logger::ptr g_logger=ZX_LOG_NAME("system");
    pid_t GetThreadid(){
        return syscall(SYS_gettid);
    }
    uint32_t GetFiberId(){
        return zx::Fiber::GetFiberId();
    }
// BackTrace函数接收一个字符串向量引用，用于存储回溯信息，size表示要捕获的栈帧数量，skip表示要跳过的栈帧数量（通常是当前函数和调用BackTrace的函数）。
void BackTrace(std::vector<std::string>& bt, int size, int skip) {
    // 分配一个指针数组，用于存储栈帧地址。
    void **array = (void**)malloc(sizeof(void*) * size);
    // 使用backtrace函数获取当前线程的调用栈，s是实际获取的栈帧数量。
    size_t s = ::backtrace(array, size);
    // 使用backtrace_symbols函数将栈帧地址转换为可读的字符串表示。
    char **strings = backtrace_symbols(array, s);
    if (strings == nullptr) {
        // 如果转换失败，记录错误日志。
        ZX_LOG_ERROR(g_logger) << "backtrace_symbols error";
        return;
    }
    // 遍历转换后的字符串数组，从skip指定的位置开始，将栈帧信息添加到字符串向量中。
    for (size_t i = skip; i < s; ++i) {
        bt.push_back(strings[i]);
    }
    // 释放分配的资源。
    free(strings);
    free(array);
}

// BacktraceToString函数用于将调用栈转换为字符串形式，size和skip参数与BackTrace函数相同，prefix是每个栈帧前的前缀字符串。
std::string BacktraceToString(int size, int skip, const std::string& prefix) {
    // 创建一个字符串向量，用于存储回溯信息。
    std::vector<std::string> bt;
    // 调用BackTrace函数填充字符串向量。
    BackTrace(bt, size, skip);
    // 创建一个字符串流，用于构建最终的字符串。
    std::stringstream ss;
    // 遍历字符串向量，将每个栈帧信息与前缀一起添加到字符串流中。
    for (size_t i = 0; i < bt.size(); ++i) {
        ss << prefix << bt[i] << std::endl;
    }
    // 将字符串流中的内容转换为字符串并返回。
    return ss.str();
}

}