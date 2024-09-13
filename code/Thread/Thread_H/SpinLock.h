#ifndef __ZX_THREAD_THREAD_H_SPINLOCK_H__
#define __ZX_THREAD_THREAD_H_SPINLOCK_H__
#include <pthread.h>
#include "Mutex.h"
namespace zx{
    class SpinLock{
        public:
        typedef ScopedLockImpl<SpinLock> Lock;
        SpinLock(){
            pthread_spin_init(&m_lock,0);
        }
        ~SpinLock(){
            pthread_spin_destroy(&m_lock);
        }
        void lock(){
            pthread_spin_lock(&m_lock);
        }
        void unlock(){
            pthread_spin_unlock(&m_lock);
        }
        private:
        pthread_spinlock_t m_lock;
    };
}
#endif