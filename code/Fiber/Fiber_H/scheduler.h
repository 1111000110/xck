#ifndef __ZX_FIBER_FIBER_H_SCHEDULER__H__
#define __ZX_FIBER_FIBER_H_SCHEDULER__H__

#include <memory>  // 包含智能指针相关的头文件
#include "fiber.h"  // 包含协程相关的头文件
#include <vector>  // 包含动态数组相关的头文件
#include <list>    // 包含双向链表相关的头文件
#include <functional>  // 包含函数封装相关的头文件
#include <iostream>
#include <atomic>  // 包含原子操作相关的头文件

// 协程调度器
namespace zx {
    class Scheduler {  // 基类
    public:
        typedef std::shared_ptr<Scheduler> ptr;  // 定义智能指针类型
        typedef Mutex MutexType;  // 定义互斥锁类型

        // 构造函数，可以指定线程数、是否使用调用者线程以及调度器名称
        Scheduler(size_t threads = 1, bool use_caller = true, const std::string& name = "");
        virtual ~Scheduler();  // 虚析构函数

        // 获取调度器名称
        const std::string& getName() const { return m_name; }
        static Scheduler* GetThis();  // 静态方法，获取当前调度器实例
        static Fiber* GetMainFiber();  // 静态方法，获取主协程

        void start();  // 启动调度器
        void stop();  // 停止调度器

        // 模板方法，用于调度协程或回调函数
        template<class FiberOrCb>
        void schedule(FiberOrCb fc, int thread = -1) {
            bool need_tickle = false;
            {
                MutexType::Lock lock(m_mutex);  // 锁定互斥锁
                need_tickle = scheduleNoLock(fc, thread);  // 调度协程或回调，不锁定
            }
            if (need_tickle) {
                tickle();  // 如果需要，唤醒某个线程
            }
        }

        // 模板方法，用于调度迭代器范围内的协程或回调函数
        template<class InputIterator>
        void schedule(InputIterator begin, InputIterator end) {
            bool need_tickle = false;
            {
                MutexType::Lock lock(m_mutex);  // 锁定互斥锁
                while (begin != end) {
                    need_tickle = scheduleNoLock(&*begin, -1) || need_tickle;  // 调度协程或回调，不锁定
                    ++begin;
                }
            }
            if (need_tickle) {
                tickle();  // 如果需要，唤醒某个线程
            }
        }

        void switchTo(int thread = -1);  // 切换到指定线程
        std::ostream& dump(std::ostream& os);  // 打印调度器状态

    protected:
        virtual void tickle();  // 虚方法，用于唤醒某个线程
        void run();  // 运行调度器
        virtual bool stopping();  // 虚方法，检查调度器是否停止
        void setThis();  // 设置当前调度器实例
        virtual void idle();  // 虚方法，当线程空闲时调用

        bool hasIdleThreads() { return m_idleThreadCount > 0; }  // 检查是否有空闲线程

    private:
        template<class FiberOrCb>
        bool scheduleNoLock(FiberOrCb fc, int thread) {//增加任务
            bool need_tickle = m_fibers.empty();
            FiberAndThread ft(fc, thread);
            if(ft.fiber || ft.cb) {
                m_fibers.push_back(ft); 
            }
            return need_tickle;
        }

    private:
        struct FiberAndThread {  // 协程和线程的结构体
            Fiber::ptr fiber;  // 协程智能指针
            std::function<void()> cb;  // 回调函数
            int thread;  // 线程ID

            // 构造函数
            FiberAndThread(Fiber::ptr f, int thr) : fiber(f), thread(thr) {}
            FiberAndThread(Fiber::ptr* f, int thr) : thread(thr) { fiber.swap(*f); }
            FiberAndThread(std::function<void()> f, int thr) : cb(f), thread(thr) {}
            FiberAndThread(std::function<void()>* f, int thr) : thread(thr) { cb.swap(*f); }
            FiberAndThread() : thread(-1) {}

            void reset() {  // 重置结构体
                fiber = nullptr;
                cb = nullptr;
                thread = -1;
            }
        };

    private:
        MutexType m_mutex;  // 互斥锁
        std::vector<Thread::ptr> m_threads;  // 线程列表
        std::list<FiberAndThread> m_fibers;  // 协程和线程的列表,任务队列
        Fiber::ptr m_rootFiber;  // 主协程
        std::string m_name;  // 调度器名称

    protected:
        std::vector<int> m_threadIds;  // 线程ID列表
        size_t m_threadCount = 0;  // 线程数量
        std::atomic<size_t> m_activeThreadCount = {0};  // 活跃线程数量
        std::atomic<size_t> m_idleThreadCount = {0};  // 空闲线程数量
        bool m_stopping = true;  // 是否停止
        bool m_autoStop = false;  // 是否主动停止
        int m_rootThread = 0;  // 主线程ID
    };

    class SchedulerSwitcher {  // 调度器切换器
    public:
        SchedulerSwitcher(Scheduler* target = nullptr);  // 构造函数
        ~SchedulerSwitcher();  // 析构函数

    private:
        Scheduler* m_caller;  // 调用者调度器
    };
}

#endif