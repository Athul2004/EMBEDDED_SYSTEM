/*
 * timer_driver.c
 * TIM2 Configuration for ADC Trigger
 */

#include "timer_driver.h"
#include "stm32_f446xx.h"

void TIM2_Init(void) {
    ENABLE_TIM2();
    
    // PSC = 0, ARR = 1999 -> 16MHz / 2000 = 8kHz
    TIM2->PSC = 0;
    TIM2->ARR = 1999;
    
    // CR2: MMS = 010 (Update)
    TIM2->CR2 &= ~(7U << 4);
    TIM2->CR2 |= (2U << 4);
    
    // Enable
    TIM2->CR1 |= 1;
}
