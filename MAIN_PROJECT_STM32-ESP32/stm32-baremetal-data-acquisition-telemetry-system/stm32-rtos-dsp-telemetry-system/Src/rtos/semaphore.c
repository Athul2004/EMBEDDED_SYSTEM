#include "rtos/semaphore.h"
#include "rtos/task.h"

void Semaphore_Init(Semaphore_t *sem, uint32_t initial_count, uint32_t max_count) {
    sem->count = initial_count;
    sem->max_count = max_count;
}

void Semaphore_Take(Semaphore_t *sem) {
    __asm volatile("cpsid i");
    while (sem->count == 0) {
        __asm volatile("cpsie i");
        Task_Yield();
        __asm volatile("cpsid i");
    }
    sem->count--;
    __asm volatile("cpsie i");
}

void Semaphore_Give(Semaphore_t *sem) {
    __asm volatile("cpsid i");
    if (sem->count < sem->max_count) {
        sem->count++;
    }
    __asm volatile("cpsie i");
}
