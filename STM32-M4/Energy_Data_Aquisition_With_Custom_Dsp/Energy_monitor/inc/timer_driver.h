/*
 * timer_driver.h
 * TIM2 Driver Header
 */

#ifndef TIMER_DRIVER_H_
#define TIMER_DRIVER_H_

#include "stm32_f446xx.h"    // Include hardware definitions

// Timer Parameters for 8kHz Trigger
// Timer Clock = 16MHz (APB1 clock)
// Target Frequency = 8000Hz (for ADC conversion rate)
// Formula: ARR = (TimerClock / ( (PSC+1) * TargetFreq )) - 1
// With PSC = 0, ARR = (16000000 / (1 * 8000)) - 1 = 1999
#define TIM2_PSC_VALUE          0U          // Prescaler value (0 means divide by 1)
#define TIM2_ARR_VALUE          1999U       // Auto-Reload Register value

// TIM CR2 Bits
#define TIM_CR2_MMS_UPDATE      (0x2U << 4) // Master Mode Selection: Update Event (TRGO). Bits 4-6 -> 010.

// TIM CR1 Bits
#define TIM_CR1_CEN             (1U << 0)   // Counter Enable bit (Bit 0)

// Function to initialize TIM2 to trigger ADC conversions
void TIM2_Init(void);

#endif /* TIMER_DRIVER_H_ */
