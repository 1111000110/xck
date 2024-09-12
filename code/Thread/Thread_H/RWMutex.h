#ifndef __ZX_THREAD_THREAD_H_RWMUTEX_H__
#define __ZX_THREAD_THREAD_H_RWMUTEX_H__
#include <pthread.h>
namespace zx{
template<class T>
class ReadScopedLockImpl{//读临时锁，RALL机制
public:
        ReadScopedLockImpl(T&mutex):m_mutex(mutex){
        m_mutex.rdlock();
        m_locked=true;
    }
    ~ ReadScopedLockImpl(){
        if(m_locked){
            m_mutex.unlock();
        }
        m_locked=false;
    }
    void lock(){
        if(!m_locked){
            m_mutex.rdlock();
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
template<class T>
class WriteScopedLockImpl{//写临时锁
    public:
    WriteScopedLockImpl(T&mutex):m_mutex(mutex){
        m_mutex.wrlock();
        m_locked=true;
    }
    ~  WriteScopedLockImpl(){
        if(m_locked){
            m_mutex.unlock();
        }
        m_locked=false;
    }
    void lock(){
        if(!m_locked){
            m_mutex.wrlock();
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
class RWMutex{
public:
    typedef ReadScopedLockImpl<RWMutex> ReadLock;
    typedef WriteScopedLockImpl<RWMutex> WriteLock;
    RWMutex(){
        pthread_rwlock_init(&m_lock,nullptr);
    }
    ~RWMutex(){
        pthread_rwlock_destroy(&m_lock);
    }
    void rdlock(){
        pthread_rwlock_rdlock(&m_lock);
    }
    void wrlock(){
        pthread_rwlock_wrlock(&m_lock);
    }
    void unlock(){
        pthread_rwlock_unlock(&m_lock);
    }
    private:
    pthread_rwlock_t m_lock;
};
}
#endif