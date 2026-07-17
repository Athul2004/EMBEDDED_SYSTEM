#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define MAX_TASKS 10

typedef void (*TaskFunction_t)(void);

typedef struct {
    TaskFunction_t taskFunc;
    uint32_t interval_ms;
    uint32_t last_run_ms;
    uint8_t active;
} Task_t;

void Scheduler_Init(void);
int8_t Scheduler_AddTask(TaskFunction_t taskFunc, uint32_t interval_ms);
void Scheduler_Run(void);

#endif // SCHEDULER_H
