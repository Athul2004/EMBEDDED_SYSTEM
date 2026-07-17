#ifndef RTOS_SEMAPHORE_H
#define RTOS_SEMAPHORE_H

#include <stdint.h>

typedef struct {
    uint32_t count;
    uint32_t max_count;
} Semaphore_t;

void Semaphore_Init(Semaphore_t *sem, uint32_t initial_count, uint32_t max_count);
void Semaphore_Take(Semaphore_t *sem);
void Semaphore_Give(Semaphore_t *sem);

#endif // RTOS_SEMAPHORE_H
