#include "../Fiber_H/scheduler.h"  // 包含调度器的头文件
#include "../../Log/Log_H/LoggerManger.h"  // 包含日志管理器的头文件
#include "../../Macro.h"  // 包含宏定义的头文件
#include "../../util.h"  // 包含实用工具函数的头文件
namespace zx {
    // 全局日志记录器
    static zx::Logger::ptr g_logger = ZX_LOG_NAME("system");
    // 线程局部存储，用于存储当前线程的调度器和协程
    static thread_local Scheduler* t_scheduler = nullptr;//此线程的调度器
    static thread_local Fiber* t_scheduler_fiber = nullptr;//此线程的主协程

    // 构造函数
    Scheduler::Scheduler(size_t threads, bool use_caller, const std::string& name)
        : m_name(name) {
        ZX_ASSERT(threads > 0);  // 确保线程数大于0
        if (use_caller) {
            zx::Fiber::GetThis();  // 协程初始化
            --threads;  // 减少线程数，因为当前线程也会作为调度器线程
            ZX_ASSERT(GetThis() == nullptr);  // 确保在调度器创建前没有其他调度器
            t_scheduler=this;
            m_rootFiber.reset(new Fiber(std::bind(&Scheduler::run, this), 0, true));  // 创建主协程
            zx::Thread::SetName(m_name);  // 设置线程名称
            t_scheduler_fiber = m_rootFiber.get();  // 存储主协程
            m_rootThread = zx::GetThreadid();  // 获取主线程ID
            m_threadIds.push_back(m_rootThread);  // 存储线程ID
        } else {
            m_rootThread = -1;  // 如果不使用调用者线程，则设置主线程ID为-1
        }
        m_threadCount = threads;  // 设置线程数量
    }

    // 析构函数
    Scheduler::~Scheduler() {
        ZX_ASSERT(m_stopping);  // 确保调度器正在停止
        if (GetThis() == this) {
            t_scheduler = nullptr;  // 清除当前线程的调度器
        }
    }

    // 获取当前线程的调度器
    Scheduler* Scheduler::GetThis() {
        return t_scheduler;
    }

    // 获取主协程
    Fiber* Scheduler::GetMainFiber() {
        return t_scheduler_fiber;
    }

    // 启动调度器
    void Scheduler::start() {
        MutexType::Lock lock(m_mutex);  // 锁定互斥锁
        if (!m_stopping) {
            return;  // 如果调度器已经在运行，则直接返回
        }
        m_stopping = false;  // 标记调度器为非停止状态
        ZX_ASSERT(m_threads.empty());  // 确保线程列表为空
        m_threads.resize(m_threadCount);  // 调整线程列表大小
        sleep(2);
        for (size_t i = 0; i < m_threadCount; ++i) {
            m_threads[i].reset(new Thread(std::bind(&Scheduler::run, this), m_name + "_" + std::to_string(i)));  // 创建新线程
            m_threadIds.push_back(m_threads[i]->getId());  // 存储线程ID
        }
        lock.unlock();  // 解锁互斥锁
    }

    // 停止调度器
    void Scheduler::stop() {
        m_autoStop = true;  // 标记调度器为主动停止
        if (m_rootFiber && m_threadCount == 0 && (m_rootFiber->getstate() == Fiber::TERM || m_rootFiber->getstate() == Fiber::INIT)) {
            ZX_LOG_INFO(g_logger) << this << " stopped";  // 日志记录停止信息
            m_stopping = true;  // 标记调度器为停止状态
            if (stopping()) {
                return;  // 如果调度器正在停止，则直接返回
            }
        }
        if (m_rootThread != -1) {
            ZX_ASSERT(GetThis() == this);  // 确保当前线程的调度器是这个调度器
        } else {
            ZX_ASSERT(GetThis() != this);  // 确保当前线程的调度器不是这个调度器
        }
        m_stopping = true;  // 标记调度器为停止状态
        for (size_t i = 0; i < m_threadCount; ++i) {
            tickle();  // 唤醒线程
        }
        if (m_rootFiber) {
            tickle();  // 唤醒主线程
        }
        if (m_rootFiber) {
            if (!stopping()) {
                m_rootFiber->call();  // 调用主协程
            }
        }
        std::vector<Thread::ptr> thrs;  // 存储线程的临时变量
        {
            MutexType::Lock lock(m_mutex);  // 锁定互斥锁
            thrs.swap(m_threads);  // 交换线程列表
        }
        for (auto& i : thrs) {
            i->join();  // 等待线程结束
        }
    }

    // 唤醒线程
    void Scheduler::tickle() {
        ZX_LOG_INFO(g_logger) << "tickle";  // 日志记录唤醒信息
    }

    // 检查调度器是否停止
    bool Scheduler::stopping() {
        MutexType::Lock lock(m_mutex);  // 锁定互斥锁
        return m_autoStop && m_stopping && m_fibers.empty() && m_activeThreadCount == 0;  // 如果满足停止条件，则返回true
    }

    // 线程空闲时调用的方法
    void Scheduler::idle() {
        ZX_LOG_INFO(g_logger) << "idle";  // 日志记录空闲信息
        while (!stopping()) {
            zx::Fiber::YieldToHold();  // 让出CPU，进入挂起状态
        }
    }

    // 设置当前线程的调度器
    void Scheduler::setThis() {
        t_scheduler = this;  // 存储当前线程的调度器
    }



    // 运行调度器
    void Scheduler::run() {
        ZX_LOG_DEBUGE(g_logger) << m_name << " run";  // 日志记录运行信息
        setThis();  // 设置当前线程的调度器
        if (zx::GetThreadid() != m_rootThread) {
            t_scheduler_fiber = Fiber::GetThis().get();  // 存储当前线程的协程
        }
        Fiber::ptr idle_fiber(new Fiber(std::bind(&Scheduler::idle, this)));  // 创建空闲协程
        Fiber::ptr cb_fiber;
        FiberAndThread ft;
        while (true) {
           
            ft.reset();
            bool tickle_me = false;
            bool is_active = false;
            {
                MutexType::Lock lock(m_mutex);  // 锁定互斥锁
                auto it = m_fibers.begin();
                while (it != m_fibers.end()) {
                    if (it->thread != -1 && it->thread != zx::GetThreadid()) {
                        ++it;
                        tickle_me = true;
                        continue;
                    }
                    ZX_ASSERT(it->fiber || it->cb);
                    if (it->fiber && it->fiber->getstate() == Fiber::EXEC) {
                        ++it;
                        continue;
                    }
                    ft = *it;
                    m_fibers.erase(it);
                    ++m_activeThreadCount;
                    is_active = true;
                    break;
                }
                tickle_me |= it != m_fibers.end();
            }
            if (tickle_me) {
                //tickle();  // 唤醒线程
            }
            
            if (ft.fiber && (ft.fiber->getstate() != Fiber::TERM && ft.fiber->getstate() != Fiber::EXCEPT)) {
                 
                ++m_activeThreadCount;
                ft.fiber->swapIn();  // 切换到协程
                --m_activeThreadCount;
                if (ft.fiber->getstate() == Fiber::READY) {
                    schedule(ft.fiber);  // 调度协程
                } else if (ft.fiber->getstate() != Fiber::TERM && ft.fiber->getstate() != Fiber::EXCEPT) {
                    ft.fiber->m_state = Fiber::HOLD;  // 挂起协程         
                   
                } 
                 ft.reset();
            }
            else if (ft.cb) {
                if (cb_fiber) {
                    cb_fiber->reset(ft.cb);  // 重置回调协程
                } else {
                    cb_fiber.reset(new Fiber(ft.cb));  // 创建新的回调协程
                }
                ft.reset();
                cb_fiber->swapIn();  // 切换到回调协程
                --m_activeThreadCount;
                if (cb_fiber->getstate() == Fiber::READY) {
                    schedule(cb_fiber);  // 调度回调协程
                    cb_fiber.reset();  // 重置回调协程指针
                } else if (cb_fiber->getstate() == Fiber::EXCEPT || cb_fiber->getstate() == Fiber::TERM) {
                    cb_fiber->reset(nullptr);  // 重置回调协程
                } else {
                    cb_fiber->m_state = Fiber::HOLD;  // 挂起回调协程
                    cb_fiber.reset();  // 重置回调协程指针
                }
            } else {
                if (is_active) {
                    --m_activeThreadCount;
                    continue;
                }
                
                if (idle_fiber->getstate() == Fiber::TERM) {
                    ZX_LOG_INFO(g_logger) << "idle fiber term";  // 日志记录空闲协程终止信息
                    break;  // 如果空闲协程终止，则退出循环
                }
                ++m_idleThreadCount;  // 增加空闲线程计数
               
                idle_fiber->swapIn();  // 切换到空闲协程
                --m_idleThreadCount;  // 减少空闲线程计数
                if (idle_fiber->getstate() != Fiber::TERM && idle_fiber->getstate() != Fiber::EXCEPT) {
                    idle_fiber->m_state = Fiber::HOLD;  // 挂起空闲协程
                }
            }
        }
    }

    // 打印调度器状态
    std::ostream& Scheduler::dump(std::ostream& os) {
        os << "[Scheduler name=" << m_name
           << " size=" << m_threadCount
           << " active_count=" << m_activeThreadCount
           << " idle_count=" << m_idleThreadCount
           << " stopping=" << m_stopping
           << " ]" << std::endl << "    ";
        for (size_t i = 0; i < m_threadIds.size(); ++i) {
            if (i) {
                os << ", ";
            }
            os << m_threadIds[i];
        }
        return os;
    }

    // 调度器切换器的构造函数
    SchedulerSwitcher::SchedulerSwitcher(Scheduler* target) {
        m_caller = Scheduler::GetThis();  // 获取当前调度器
        if (target) {
            target->switchTo();  // 切换到目标调度器
        }
    }

    // 调度器切换器的析构函数
    SchedulerSwitcher::~SchedulerSwitcher() {
        if (m_caller) {
            m_caller->switchTo();  // 切换回原来的调度器
        }
    }
        // 切换到指定线程
    void Scheduler::switchTo(int thread) {
        ZX_ASSERT(Scheduler::GetThis() != nullptr);  // 确保当前线程的调度器不为空
        if (Scheduler::GetThis() == this) {
            if (thread == -1 || thread == zx::GetThreadid()) {
                return;  // 如果目标线程是当前线程，则直接返回
            }
        }
        schedule(Fiber::GetThis(), thread);  // 调度当前协程到指定线程
        Fiber::YieldToHold();  // 让出CPU，进入挂起状态
    }
}