#include "../Thread_H/Thread.h"

namespace zx{
    static thread_local Thread* t_thread = nullptr; // 定义一个线程局部变量，用于存储当前线程的Thread对象指针。
    static thread_local std::string t_thread_name = "UNKNOW"; // 定义一个线程局部变量，用于存储当前线程的名称。
    static zx::Logger::ptr g_logger = ZX_LOG_NAME("system");  // 定义一个全局日志对象指针，用于日志记录。

    // 设置当前线程的名称。
    void Thread::SetName(const std::string& name) {
        // 如果当前线程的Thread对象指针不为空，则更新其名称。
        if (t_thread) {
            t_thread->m_name = name;
        }
        // 更新线程名称的线程局部变量。
        t_thread_name = name;
    }

    // 线程的运行函数，由pthread_create调用。
    void* Thread::run(void* arg) {
        // 从参数中获取Thread对象。
        Thread* thread = (Thread*)arg;
        // 设置当前线程的Thread对象指针为当前线程。
        t_thread = thread;
        // 获取当前线程的系统ID。
        thread->m_id = GetThreadid();
        // 设置线程的名称，名称长度限制为15个字符。
        pthread_setname_np(pthread_self(), thread->m_name.substr(0, 15).c_str());
        // 交换回调函数，避免在执行回调时被销毁。
        std::function<void()> cb;
        cb.swap(thread->m_cb);
        // 执行回调函数。
        thread->m_semaphore.notify();
        cb();
        return 0;
    }

    // Thread类的构造函数，接受一个函数和线程名称。
    Thread::Thread(std::function<void()> cv, const std::string& name):m_cb(cv),m_name(name),m_semaphore(0){
        // 如果提供的名称为空，则默认为"UNKNOW"。
        if (name.empty()) {
            m_name = "UNKNOW";
        } else {
            m_name = name;
        }
        // 创建线程，如果失败则记录错误并抛出异常。
        int rt = pthread_create(&m_thread, nullptr, &Thread::run, this);
        if (rt) {
            ZX_LOG_ERROR(g_logger) << "pthread_create thread fail, rt=" << rt << " name=" << name;
            throw std::logic_error("pthread_create error");
        }  
        m_semaphore.wait();
    }

    // Thread类的析构函数，用于清理资源。
    Thread::~Thread() {
        // 如果线程存在，则分离线程，释放资源。
        if (m_thread) {
            pthread_detach(m_thread);
        }
    }

    // 等待线程结束。
    void Thread::join() {
        // 如果线程存在，则等待线程结束。
        if (m_thread) {
            int rt = pthread_join(m_thread, nullptr);
            // 如果等待失败，则记录错误并抛出异常。
            if (rt) {
                ZX_LOG_ERROR(g_logger) << "pthread_join thread fail, rt=" << rt << " name=" << m_name;
                throw std::logic_error("pthread_join error");
            }
            // 清除线程ID，表示线程已结束。
            m_thread = 0;
        }
    }

    // 获取当前线程的Thread对象。
    Thread* Thread::GetThis() {
        return t_thread;
    }

    // 获取当前线程的名称。
    const std::string& Thread::GetName() {
        return t_thread_name;
    }
}