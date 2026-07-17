#ifndef RTOS_KERNEL_H
#define RTOS_KERNEL_H

#include <stdint.h>

void Kernel_Init(void);
void Kernel_Start(void);
uint32_t Kernel_GetTickCount(void);
void Kernel_SysTickHandler(void);

#endif // RTOS_KERNEL_H
