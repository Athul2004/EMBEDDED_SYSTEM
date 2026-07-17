#include "rtos/syscall.h"
#include "rtos/kernel.h"

void Syscall_Init(void) {
    // Initialize SVC interrupts if using MPU or privileged levels
}

// Inline assembly to trigger SVC
uint32_t __attribute__((naked)) Syscall_GetTick(void) {
    __asm volatile (
        "SVC 1 \n"
        "BX LR \n"
    );
}

void SVC_Handler(void) {
    // Basic SVC handler framework
    // Would normally decode SVC number from stacked PC
}
