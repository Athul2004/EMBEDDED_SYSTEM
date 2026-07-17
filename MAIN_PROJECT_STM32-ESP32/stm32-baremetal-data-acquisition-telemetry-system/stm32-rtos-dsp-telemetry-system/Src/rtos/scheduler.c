#include "rtos/scheduler.h"
#include "drivers/delay.h" // For SysTick_GetMillis()
#include <stddef.h>

static Task_t tasks[MAX_TASKS];
static uint8_t task_count = 0;

void Scheduler_Init(void) {
    task_count = 0;
    for (int i = 0; i < MAX_TASKS; i++) {
        tasks[i].active = 0;
    }
}

int8_t Scheduler_AddTask(TaskFunction_t taskFunc, uint32_t interval_ms) {
    if (task_count >= MAX_TASKS) {
        return -1; // Scheduler full
    }
    
    tasks[task_count].taskFunc = taskFunc;
    tasks[task_count].interval_ms = interval_ms;
    tasks[task_count].last_run_ms = SysTick_GetMillis(); // Initialize to current time
    tasks[task_count].active = 1;
    
    task_count++;
    return task_count - 1;
}

void Scheduler_Run(void) {
    while (1) {
        uint32_t current_time = SysTick_GetMillis();
        
        for (uint8_t i = 0; i < task_count; i++) {
            if (tasks[i].active) {
                // Check if the interval has elapsed
                if ((current_time - tasks[i].last_run_ms) >= tasks[i].interval_ms) {
                    // Update last run time
                    tasks[i].last_run_ms = current_time;
                    
                    // Execute the task
                    if (tasks[i].taskFunc != NULL) {
                        tasks[i].taskFunc();
                    }
                }
            }
        }
    }
}
