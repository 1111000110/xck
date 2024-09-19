#ifndef __ZX_THREAD_THREAD_H_THREAD_H__
#define __ZX_THREAD_THREAD_H_THREAD_H__
#include <thread>
#include <memory>
#include <functional>
#include <pthread.h>
#include "../../util.h"

#include "Semaphore.h"
#include "Mutex.h"
#include "RWMutex.h"
//std::thread 没有实现读写锁，自己实现
namespace zx{
    class Thread {
        public:
        typedef std::shared_ptr<Thread> ptr;
        // 构造函数，接受一个函数对象和线程名称
        // cv: 函数对象，将在新线程中执行
        // name: 线程名称
        Thread(std::function<void()> cv, const std::string& name);
        ~Thread();// 析构函数，虚析构函数以支持子类化
        const std::string& getName() const { return m_name; }// 获取线程名称
        pid_t getId() const { return m_id; }// 获取线程ID
        void join();// 等待线程结束
        static Thread* GetThis();// 获取当前线程对象的静态方法
        static const std::string& GetName();  // 获取当前线程的名称的静态方法，用于日志记录
        static void SetName(const std::string& name);// 设置当前线程的名称的静态方法

    private:
        Thread(const Thread&) = delete;
        Thread(const Thread&&) = delete;
        Thread& operator=(const Thread&) = delete;
        static void* run(void* arg); // 线程的运行函数，用于 pthread_create
    private:
        pid_t m_id = -1;// 线程ID，初始化为-1表示无效           
        pthread_t m_thread = 0; // 线程句柄，用于 pthread 库
        std::function<void()> m_cb;  // 需要执行的函数对象
        std::string m_name;// 线程名称
        Semaphore m_semaphore;//信号量    
    };
}
#endif 