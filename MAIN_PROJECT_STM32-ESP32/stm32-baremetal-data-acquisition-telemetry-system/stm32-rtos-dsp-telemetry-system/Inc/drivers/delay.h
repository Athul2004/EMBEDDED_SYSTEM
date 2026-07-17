#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>

#define SysTick_BASE        0xE000E010UL
typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;
} SysTick_TypeDef;
#define SysTick             ((SysTick_TypeDef *)SysTick_BASE)

#define SCB_CPACR           (*((volatile uint32_t *)0xE000ED88U))

void SysTick_Init(void);
void delay_ms(uint32_t ms);
uint32_t SysTick_GetMillis(void);

#endif // DELAY_H
