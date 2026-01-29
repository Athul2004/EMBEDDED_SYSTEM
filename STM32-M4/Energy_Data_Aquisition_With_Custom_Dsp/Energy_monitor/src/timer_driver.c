/*
 * timer_driver.c
 * TIM2 Configuration Implementation
 */

#include "timer_driver.h"

/*
 * @brief  Initializes TIM2 to trigger ADC conversions at approx 8kHz
 * @param  None
 * @retval None
 */
void TIM2_Init(void) {
    // 1. Enable Clock for TIM2 Peripheral (APB1)
    ENABLE_TIM2();
    
    // 2. Configure Time Base
    // Timer Clock = PCLK1 = 16MHz (assuming default HSI/APB1 prescalers)
    // Target Frequency = 8000 Hz
    
    // PSC (Prescaler): Divide clock by PSC+1
    TIM2->PSC = TIM2_PSC_VALUE;
    
    // ARR (Auto-Reload Register): Counter counts up to ARR then resets. 
    TIM2->ARR = TIM2_ARR_VALUE;
    
    // 3. Configure Trigger Output (TRGO)
    // CR2 (Control Register 2) MMS Bits (Master Mode Selection)
    // We want TRGO signal to pulse on Update Event (UEV - Counter Overflow)
    // Clear MMS bits (4-6) first then set
    TIM2->CR2 &= ~(7U << 4);
    TIM2->CR2 |= TIM_CR2_MMS_UPDATE;
    
    // 4. Enable Timer
    TIM2->CR1 |= TIM_CR1_CEN;
}
