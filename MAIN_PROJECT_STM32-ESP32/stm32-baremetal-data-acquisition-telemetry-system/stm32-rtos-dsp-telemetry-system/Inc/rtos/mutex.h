#ifndef RTOS_MUTEX_H
#define RTOS_MUTEX_H

#include "rtos/task.h"
#include <stdint.h>

typedef struct {
    uint8_t locked;
    TCB_t *owner;
} Mutex_t;

void Mutex_Init(Mutex_t *mutex);
void Mutex_Lock(Mutex_t *mutex);
void Mutex_Unlock(Mutex_t *mutex);

#endif // RTOS_MUTEX_H
