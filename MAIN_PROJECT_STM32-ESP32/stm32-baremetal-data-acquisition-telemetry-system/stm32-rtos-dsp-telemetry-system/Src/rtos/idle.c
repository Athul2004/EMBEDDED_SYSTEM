#include "rtos/idle.h"
#include "rtos/task.h"

static void Idle_Task(void) {
    while(1) {
        // WFI: Wait For Interrupt (Sleep)
        __asm volatile ("WFI");
    }
}

void Idle_Init(void) {
    // 0xFF used as lowest priority
    Task_Create(Idle_Task, 0xFF); 
}
