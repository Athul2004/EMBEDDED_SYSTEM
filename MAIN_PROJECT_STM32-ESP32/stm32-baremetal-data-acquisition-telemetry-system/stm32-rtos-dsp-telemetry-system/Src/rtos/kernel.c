#include "rtos/kernel.h"
#include "rtos/task.h"
#include <stdint.h>

#define SCB_ICSR        (*((volatile uint32_t *)0xE000ED04))
#define SCB_ICSR_PENDSVSET (1 << 28)

#define NVIC_SYSPRI3    (*((volatile uint32_t *)0xE000ED20))

volatile uint32_t rtos_tick_count = 0;
volatile uint8_t kernel_started = 0;

void Kernel_Init(void) {
    Task_InitSystem();
    kernel_started = 0;
}

void Kernel_Start(void) {
    // Set PendSV to lowest priority (0xFF)
    NVIC_SYSPRI3 |= (0xFF << 16); 

    kernel_started = 1;

    // Start first task
    Task_StartFirst();
}

uint32_t Kernel_GetTickCount(void) {
    return rtos_tick_count;
}

void Kernel_SysTickHandler(void) {
    rtos_tick_count++;
    if (kernel_started) {
        Task_UpdateDelays();
        // Trigger Context Switch via PendSV
        SCB_ICSR |= SCB_ICSR_PENDSVSET;
    }
}
