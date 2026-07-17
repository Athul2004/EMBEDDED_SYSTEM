#include "drivers/delay.h"
#include "rtos/kernel.h"

// Assuming default 16 MHz HSI clock for SysTick
#define SYSTEM_CORE_CLOCK 16000000

volatile uint32_t ms_ticks = 0;

void SysTick_Handler(void) {
    ms_ticks++;
    Kernel_SysTickHandler();
}

void SysTick_Init(void) {
    // 1ms tick
    SysTick->LOAD = (SYSTEM_CORE_CLOCK / 1000) - 1;
    SysTick->VAL = 0;
    // Enable SysTick interrupt, use processor clock, enable SysTick
    SysTick->CTRL = (1 << 1) | (1 << 2) | (1 << 0); 
}

void delay_ms(uint32_t ms) {
    uint32_t start = ms_ticks;
    while ((ms_ticks - start) < ms);
}

uint32_t SysTick_GetMillis(void) {
    return ms_ticks;
}
