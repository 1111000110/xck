
#ifndef __ZX_THRED_TREHD_H_SEMAPHORE_H__
#define __ZX_THRED_TREHD_H_SEMAPHORE_H__
#include <pthread.h>
#include <stdint.h>
#include <semaphore.h>
#include "../../Log/Log_H/LoggerManger.h"
#include <memory>
namespace zx{
    class Semaphore{
        public:
            Semaphore(uint32_t count=0);
            ~Semaphore();
            void wait();
            void notify();
        private:
            Semaphore(const Semaphore&)=delete;
            Semaphore(Semaphore&&)=delete;
            Semaphore& operator=(const Semaphore&)=delete;
        private:
            sem_t m_semaphore;
    };
}
#endif