#ifndef __ZX_THREAD_THREAD_H_MUTEX_H__
#define __ZX_THREAD_THREAD_H_MUTEX_H__

#include <mutex>
namespace zx{
template<class T>
class ScopedLockImpl{//临时锁，RALL机制
public:
    ScopedLockImpl(T&mutex):m_mutex(mutex){
        m_mutex.lock();
        m_locked=true;
    }
    ~ScopedLockImpl(){
        if(m_locked){
            m_mutex.unlock();
        }
        m_locked=false;
    }
    void lock(){
        if(!m_locked){
            m_mutex.lock();
        }
        m_locked=true;
    }
    void unlock(){
        if(m_locked){
            m_mutex.unlock();
        }
        m_locked=false;
    }
    private:
        T&m_mutex;
        bool m_locked;
};

class Mutex{//互斥锁
   
    public:
     typedef ScopedLockImpl<Mutex> Lock;
        Mutex(){
            pthread_mutex_init(&m_mutex,nullptr);
        }
        ~Mutex(){
            pthread_mutex_destroy(&m_mutex);
        }
        void lock(){
            pthread_mutex_lock(&m_mutex);
        }
        void unlock(){
            pthread_mutex_unlock(&m_mutex);
        }
    private:
        pthread_mutex_t m_mutex;
};
}
#endif