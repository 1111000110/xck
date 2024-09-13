#ifndef __ZX_THREAD_THREAD_H_CASLOCK_H__
#define __ZX_THREAD_THREAD_H_CASLOCK_H__
#include <atomic>
namespace zx{
    class CASLock{
        CASLock(){
            m_mutex.clear();
        }
        ~CASLock(){

        }
        void lock(){
            while(std::atomic_flag_test_and_set_explicit(&m_mutex,std::memory_order_acquire));
        }
        void unlock(){
            std::atomic_flag_clear_explicit(&m_mutex,std::memory_order_release);
        }
        private:
        volatile std::atomic_flag m_mutex;
    };
}
#endif