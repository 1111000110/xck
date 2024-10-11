
#include "../Fiber_H/fiber.h" // 包含Fiber类的定义
#include <atomic> // 包含原子操作库，用于线程安全的计数器
#include "../../Macro.h" // 包含宏定义，可能包含断言等宏
#include "../../Config/Config_H/Config.h" // 包含配置管理类的定义
#include "../../Log/Log_H/LoggerManger.h" // 包含日志库
#include "../Fiber_H/scheduler.h"
namespace zx{
    zx::Logger::ptr g_logger=zx::LoggerManger::LoggerMgr::GetInstance()->getLogger("system"); // 创建一个全局的Logger对象，用于日志记录
    static std::atomic<u_int64_t> s_fiber_id{0};//定义一个原子的uint64_t变量，用于生成唯一的协程ID
    static std::atomic<u_int64_t> s_fiber_count{0}; // 定义一个原子的uint64_t变量，用于跟踪当前活动的协程数量
    static thread_local Fiber* t_fiber =nullptr;// 定义线程局部存储，用于存储指向当前协程的指针
    static thread_local Fiber::ptr t_threadFiber=nullptr;  // 定义线程局部存储，用于存储指向当前线程的主协程智能指针
    static ConfigVar<uint32_t>::ptr g_fiber_stack_size=Config::Lookup<uint32_t>("fiber.stack_size",1024*1024,"fiber stack size");// 从配置管理器中获取协程栈大小的配置变量

    // 定义一个栈内存分配器类，用于分配和释放栈内存
    class MallocStackAllocater{
        public:
            // 分配指定大小的内存
            static void* Alloc(size_t size){
                return malloc(size);
            }
            // 释放指定的内存
            static void Dealloc(void* vp, size_t size){
                return free(vp);
            }
    };
    // 使用MallocStackAllocater作为栈分配器
    using StackAllocator=MallocStackAllocater;
    void Fiber::call(){//当某个协程调度时，将会将线程局部变量的当前协程设置为自己
        SetThis(this);//设置自己为当前调度的协程
        m_state=EXEC;//自己的状态改为运行
        if(swapcontext(&(*t_threadFiber).m_ctx,&m_ctx)){//将当前上下文存在主协程中，调度自己协程的上下文
            ZX_ASSERT2(false,"swapcontext");
        }
    }
    // Fiber类的构造函数，初始化主协程
    Fiber::Fiber(){
        m_state=EXEC; // 设置状态为执行中
        SetThis(this); // 设置当前协程为这个新创建的协程
        if(getcontext(&m_ctx)){ // 获取当前上下文
            ZX_ASSERT2(false,"getcontext"); // 如果失败，断言失败
        }
        ++s_fiber_count; // 增加协程计数
        ZX_LOG_DEBUGE(g_logger) << "Fiber::Fiber main";
    }

    // 设置当前协程的函数
    void Fiber::SetThis(Fiber* t){
        t_fiber=t; // 将线程局部变量t_fiber设置为传入的协程指针
    }

    // Fiber类的构造函数，用于创建新的协程
    Fiber::Fiber(std::function<void()> cb, size_t stacksize,bool use_callor):m_id(++s_fiber_id), m_cb(cb){
        ++s_fiber_count; // 增加协程计数
        m_stacksize=stacksize?stacksize:g_fiber_stack_size->getValue(); // 设置栈大小，如果未指定，则使用配置值
        m_stack=StackAllocator::Alloc(m_stacksize); // 分配栈内存
        if(getcontext(&m_ctx)){ // 获取当前上下文
             ZX_ASSERT2(false,"getcontext"); // 如果失败，断言失败
        }
        m_ctx.uc_link=nullptr; // 设置链接上下文为空
        m_ctx.uc_stack.ss_sp=m_stack; // 设置栈指针
        m_ctx.uc_stack.ss_size=m_stacksize; // 设置栈大小
        if(!use_callor){
                  
            makecontext(&m_ctx,&Fiber::MainFunc,0);
        }
        else{
            makecontext(&m_ctx, &Fiber::CallerMainFunc, 0); // 创建上下文，设置主函数
        }
        ZX_LOG_DEBUGE(g_logger) << "Fiber::Fiber id=" << m_id;
    }
     void Fiber::CallerMainFunc(){
        Fiber::ptr cur=GetThis(); // 获取当前协程的智能指针
        ZX_ASSERT(cur); // 断言当前协程不为空
        try{
            cur->m_cb(); // 执行回调函数
            cur->m_cb=nullptr; // 清空回调函数
            cur->m_state=TERM; // 设置状态为终止
        }catch(std::exception& ex){
            cur->m_state=EXCEPT; // 设置状态为异常终止
            ZX_LOG_ERROR(g_logger) << "Fiber Except: " << ex.what()
            << " fiber_id=" << cur->GetId()
            << std::endl
            << zx::BacktraceToString();    
            }
        catch(...){
            cur->m_state=EXCEPT; // 设置状态为异常终止
                  ZX_LOG_ERROR(g_logger) << "Fiber Except"
            << " fiber_id=" << cur->GetId()
            << std::endl
            << zx::BacktraceToString();
        }
        auto raw_ptr=cur.get(); // 获取当前协程的原始指针
        cur.reset();
        raw_ptr->back(); // 切换到其他协程执行
        ZX_ASSERT2(false, "never reach fiber_id=" + std::to_string(raw_ptr->GetId()));
     }
    // Fiber类的析构函数，用于清理协程资源
    Fiber::~Fiber(){
        --s_fiber_count; // 减少协程计数
        if(m_stack){ // 如果栈内存已分配
            ZX_ASSERT(m_state==TERM||m_state==EXCEPT||m_state==INIT); // 断言状态
            StackAllocator::Dealloc(m_stack, m_stacksize); // 释放栈内存
        }else{
            ZX_ASSERT(!m_cb); // 断言回调函数为空
            ZX_ASSERT(m_state==EXEC); // 断言状态
            Fiber* cur=t_fiber; // 获取当前协程
            if(cur==this){ // 如果当前协程是这个协程
                SetThis(nullptr); // 设置当前协程为空
            }
        }
         ZX_LOG_DEBUGE(g_logger) << "Fiber::~Fiber id=" << m_id
                              << " total=" << s_fiber_count;
    }

    // 重置协程函数，并重置状态
    void Fiber::reset(std::function<void()> cb){
        ZX_ASSERT(m_stack); // 断言栈内存已分配
        ZX_ASSERT(m_state==TERM||m_state==EXCEPT||m_state==INIT); // 断言状态
        m_cb=cb; // 设置新的回调函数
        if(getcontext(&m_ctx)){ // 获取当前上下文
            ZX_ASSERT2(false,"getcontext"); // 如果失败，断言失败
        }
        m_ctx.uc_link=nullptr; // 设置链接上下文为空
        m_ctx.uc_stack.ss_sp=m_stack; // 设置栈指针
        m_ctx.uc_stack.ss_size=m_stacksize; // 设置栈大小
        makecontext(&m_ctx, &Fiber::MainFunc, 0); // 创建上下文，设置主函数
        m_state=INIT; // 设置状态为初始化
    }
    // 获取当前协程的ID
    u_int64_t Fiber::GetFiberId(){
        if(t_fiber){ // 如果当前协程不为空
            return t_fiber->GetId(); // 返回当前协程的ID
        }
        return 0; // 如果当前协程为空，返回0
    }

    // 切换到当前协程执行
    void Fiber::swapIn(){
        SetThis(this); // 设置当前协程
        ZX_ASSERT(m_state!=EXEC); // 断言状态不是执行中
        m_state=EXEC; // 设置状态为执行中
        if(swapcontext(&Scheduler::GetMainFiber()->m_ctx, &m_ctx)){ // 切换上下文
            ZX_ASSERT2(false,"swapcontext"); // 如果失败，断言失败
        }
    }
    void Fiber::back(){
        SetThis(t_threadFiber.get());
        if(swapcontext(&m_ctx,&t_threadFiber->m_ctx)){
            ZX_ASSERT2(false,"swapcontext");
        }
    }
    // 切换到后台执行
    void Fiber::swapOut(){
         SetThis(Scheduler::GetMainFiber());
        if(swapcontext(&m_ctx, &Scheduler::GetMainFiber()->m_ctx)) {
           ZX_ASSERT2(false, "swapcontext");
        }
    }

    // 获取当前协程的智能指针
    Fiber::ptr Fiber::GetThis(){
        if(t_fiber){ // 如果当前协程不为空
            return t_fiber->shared_from_this(); // 返回当前协程的智能指针
        }
        Fiber::ptr main_fiber(new Fiber()); // 创建一个新的主协程
        ZX_ASSERT(t_fiber==main_fiber.get()); // 断言当前协程是新创建的主协程
        t_threadFiber=main_fiber; // 将新创建的主协程设置为线程局部变量中的协程
        return t_fiber->shared_from_this(); // 返回当前协程的智能指针
    }

    // 协程切换到后台，并且设置为Ready状态
    void Fiber::YieldToReady(){
        Fiber::ptr cur=GetThis(); // 获取当前协程的智能指针

        ZX_ASSERT(cur->m_state == EXEC);
        cur->m_state=READY; // 设置状态为就绪
        cur->back(); // 切换到后台
    }

    // 协程切换到后台，并且设置为Hold状态
    void Fiber::YieldToHold(){
        Fiber::ptr cur=GetThis(); // 获取当前协程的智能指针
         ZX_ASSERT(cur->m_state == EXEC);
        cur->m_state=HOLD; // 设置状态为暂停
        cur->swapOut(); // 切换到后台
    }

    // 获取总协程数
    uint64_t Fiber::TotalFibers(){
        return s_fiber_count; // 返回协程计数
    }

    // 协程的主函数，用于执行协程的回调函数
    void Fiber::MainFunc(){
        Fiber::ptr cur=GetThis(); // 获取当前协程的智能指针
        ZX_ASSERT(cur); // 断言当前协程不为空
        try{
            cur->m_cb(); // 执行回调函数
            cur->m_cb=nullptr; // 清空回调函数
            cur->m_state=TERM; // 设置状态为终止
        }catch(std::exception& ex){
            cur->m_state=EXCEPT; // 设置状态为异常终止
           ZX_LOG_ERROR(g_logger) << "Fiber Except: " << ex.what()
            << " fiber_id=" << cur->GetId()
            << std::endl
            << zx::BacktraceToString();        
        }
        catch(...){
            cur->m_state=EXCEPT; // 设置状态为异常终止
           ZX_LOG_ERROR(g_logger) << "Fiber Except"
            << " fiber_id=" << cur->GetId()
            << std::endl
            << zx::BacktraceToString();      }
        auto raw_ptr=cur.get(); // 获取当前协程的原始指针
        cur.reset();
        raw_ptr->swapOut(); // 切换到其他协程执行
         ZX_ASSERT2(false, "never reach fiber_id=" + std::to_string(raw_ptr->GetId()));
    }
}


