/*
 * timer_driver.h
 * TIM2 Driver Header
 */

#ifndef TIMER_DRIVER_H_
#define TIMER_DRIVER_H_

#include "stm32_f446xx.h"

// Timer Parameters for 8kHz Trigger
// Timer Clock = 16MHz
// F_target = 8000Hz
// ARR = (16000000 / ( (PSC+1) * 8000 )) - 1
// With PSC = 0, ARR = 1999 + 1 = 2000
#define TIM2_PSC_VALUE          0U
#define TIM2_ARR_VALUE          1999U

// TIM CR2 Bits
#define TIM_CR2_MMS_UPDATE      (0x2U << 4) // Master Mode Selection: Update Event (TRGO)

// TIM CR1 Bits
#define TIM_CR1_CEN             (1U << 0)   // Counter Enable

void TIM2_Init(void);

#endif /* TIMER_DRIVER_H_ */
