#ifndef __ZX_IOMANAGER_H__
#define __ZX_IOMANAGER_H__
#include "scheduler.h"
#include "../../Macro.h"
#include <sys/epoll.h>
namespace zx{
    class IOManager:public Scheduler{//IO管理器
        public:
            typedef std::shared_ptr<IOManager>ptr;//智能指针
            typedef RWMutex RWMutexType;//读写锁
            enum Event{//事件类型
                NONE  =0x0,
                READ  =0x1,
                WRITE = 0x4,
            };
        private:
            struct FdContext{//句柄
                typedef Mutex MutexType;//锁
                struct EventContext{//调度
                    Scheduler*scheduler=nullptr;//待执行的scheduler
                    Fiber::ptr fiber;           //事件协程
                    std::function<void()>cb;    //事件回调函数
                };
                    int fd;//事件关联的句柄·
                    EventContext read;//读事件
                    EventContext write;//写事件
                    Event m_events=NONE;//已经注册的事件
                    MutexType mutex;
            };
        public:
            IOManager(size_t threads = 1, bool use_caller = true, const std::string& name = "");
            ~IOManager();
            //1 success 0 retry -1 error
            int addEvent(int fd,Event event,std::function<void()>cb=nullptr);
            bool delEvent(int fd,Event event);
            bool cancelEvent(int fd,Event event);//取消事件
            bool cancelAll(int fd);
            static IOManager*GetThis();
        protected:
            void tickle()override;
            bool stopping()override;
            void idle()override;
            void contextResize(size_t size);
        private:
            int m_epfd=0;
            int m_tickleFds[2];//管道0读1写
            std::atomic<size_t>m_pendingEventCount={0};//等待执行的事件数量
            RWMutexType m_mutex;
            std::vector<FdContext*>m_fdContexts;//句柄数组
    };
}
#endif