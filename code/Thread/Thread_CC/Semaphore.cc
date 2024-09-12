#include "../Thread_H/Semaphore.h"
namespace zx{
    zx::Logger::ptr logger=ZX_LOG_ROOT();   
    Semaphore::Semaphore(uint32_t count){
        if(sem_init(&m_semaphore,0,count)){
            ZX_LOG_INFO(logger)<<"sem_init error";
            throw std::logic_error("sem_init error");
        }
    }
    Semaphore:: ~Semaphore(){
        sem_destroy(&m_semaphore);
    }
    void  Semaphore::wait(){
        if(sem_wait(&m_semaphore)){
         throw std::logic_error("sem_wait error");
        }
    }
    void  Semaphore::notify(){
        if(sem_post(&m_semaphore)){
            throw std::logic_error("sem_post error");
        }
    }    
}