#ifndef RTOS_TASK_H
#define RTOS_TASK_H

#include <stdint.h>

#define MAX_TASKS 16
#define STACK_SIZE 256 // 256 words = 1KB per task

typedef enum {
    TASK_STATE_READY,
    TASK_STATE_RUNNING,
    TASK_STATE_BLOCKED,
    TASK_STATE_SUSPENDED
} TaskState_t;

typedef struct {
    uint32_t *sp;             // Stack pointer (must be first element for ASM)
    uint32_t stack[STACK_SIZE]; // Task stack
    TaskState_t state;
    uint32_t priority;
    uint32_t delay_ticks;
} TCB_t;

void Task_InitSystem(void);
int Task_Create(void (*task_func)(void), uint32_t priority);
void Task_StartFirst(void);
void Task_UpdateDelays(void);
void Task_Delay(uint32_t ticks);
void Task_Yield(void);

#endif // RTOS_TASK_H
