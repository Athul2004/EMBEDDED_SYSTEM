#ifndef RTOS_SYSCALL_H
#define RTOS_SYSCALL_H

#include <stdint.h>

void Syscall_Init(void);
uint32_t Syscall_GetTick(void);

#endif // RTOS_SYSCALL_H
