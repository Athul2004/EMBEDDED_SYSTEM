#include "rtos/mutex.h"
#include <stddef.h>

void Mutex_Init(Mutex_t *mutex) {
    mutex->locked = 0;
    mutex->owner = NULL;
}

void Mutex_Lock(Mutex_t *mutex) {
    extern TCB_t *current_tcb;
    __asm volatile("cpsid i"); 
    
    while (mutex->locked) {
        __asm volatile("cpsie i");
        Task_Yield();
        __asm volatile("cpsid i");
    }
    
    mutex->locked = 1;
    mutex->owner = current_tcb;
    
    __asm volatile("cpsie i");
}

void Mutex_Unlock(Mutex_t *mutex) {
    extern TCB_t *current_tcb;
    __asm volatile("cpsid i");
    
    if (mutex->locked && mutex->owner == current_tcb) {
        mutex->locked = 0;
        mutex->owner = NULL;
    }
    
    __asm volatile("cpsie i");
}
