#ifndef __ZX_FIBER_H__ // 如果没有定义__ZX_FIBER_H__，则定义它，防止头文件被重复包含
#define __ZX_FIBER_H__
#include <ucontext.h> // 包含ucontext库，用于协程的上下文切换
#include <functional> // 包含functional库，用于存储函数对象
#include <memory> // 包含memory库，用于智能指针等
#include "../../Thread/Thread_H/Thread.h" // 包含线程库


// 定义命名空间zx，用于封装协程相关的类和函数
namespace zx{
    class Scheduler;
   
    // 定义Fiber类，继承自std::enable_shared_from_this，使得可以在成员函数中获取当前对象的智能指针
    class Fiber:public std::enable_shared_from_this<Fiber>{
        public:
         friend class Scheduler;
        typedef std::shared_ptr<Fiber> ptr; // 定义智能指针类型
        enum State{ // 定义协程的状态枚举
            INIT, // 初始化
            HOLD, // 暂停
            EXEC, // 执行
            TERM, // 结束
            READY, // 就绪
            EXCEPT // 异常
        };
        private:
            Fiber(); // 私有构造函数，用于初始化主协程
        public:
            // 公共构造函数，接受一个函数对象和栈大小，用于创建新的协程
            Fiber(std::function<void()> cb, size_t stacksize=0,bool ues_callor=false);
            ~Fiber(); // 析构函数，用于清理协程资源
            // 重置协程函数，并重置状态
            void reset(std::function<void()> cb); // 参数为新的函数对象
            void swapIn();  // 切换到当前协程执行
            void swapOut(); // 切换到其他协程
            void call();
            void back();
            uint64_t GetId() const { return m_id; } // 获取协程ID
            State getstate()const{return m_state;}
        public:
            static void SetThis(Fiber* f); // 参数为协程指针
            static Fiber::ptr GetThis(); // 返回当前协程的智能指针
            static void YieldToReady(); // 将当前协程设置为就绪状态
            static void YieldToHold(); // 将当前协程设置为阻塞状态
            static uint64_t TotalFibers(); // 返回总协程数
            static void MainFunc(); // 协程的主函数，用于执行协程的回调函数
            static void CallerMainFunc(); // 协程的主函数，用于执行协程的回调函数
            static u_int64_t GetFiberId(); // 获取当前协程的ID
            
        private:
            uint64_t m_id=0; // 协程id
            uint32_t m_stacksize=0; // 栈大小
            //State m_state=INIT; // 协程状态
            State m_state=INIT; // 协程状态
            ucontext_t m_ctx; // 协程上下文
            void *m_stack=nullptr; // 协程栈
            std::function<void()> m_cb; // 协程回调函数
    };
}
#endif