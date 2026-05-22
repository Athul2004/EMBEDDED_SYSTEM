/*
 * timer_driver.c
 * TIM2 Configuration Implementation
 */

#include "timer_driver.h"   // Include timer driver header

/*
 * @brief  Initializes TIM2 to trigger ADC conversions at approx 8kHz
 * @param  None
 * @retval None
 */
void TIM2_Init(void) {
    // 1. Enable Clock for TIM2 Peripheral (APB1 Bus)
    ENABLE_TIM2();
    
    // 2. Configure Time Base
    // Timer Clock = PCLK1 = 16MHz (assuming default HSI/APB1 prescalers)
    // Target Frequency = 8000 Hz
    
    // PSC (Prescaler): Divide clock by PSC+1
    // 0 means no division, count at full 16MHz
    TIM2->PSC = TIM2_PSC_VALUE;
    
    // ARR (Auto-Reload Register): Counter counts up to ARR then resets. 
    // This defines the period of the timer.
    // 8000Hz = 16MHz / (1999 + 1)
    TIM2->ARR = TIM2_ARR_VALUE;
    
    // 3. Configure Trigger Output (TRGO)
    // CR2 (Control Register 2) MMS Bits (Master Mode Selection)
    // We want TRGO signal to pulse on Update Event (UEV - Counter Overflow)
    // This pulse is internally connected to ADC Trigger.
    
    // Clear MMS bits (4-6) first then set
    TIM2->CR2 &= ~(7U << 4);
    // Set MMS = 010 (Update)
    TIM2->CR2 |= TIM_CR2_MMS_UPDATE;
    
    // 4. Enable Timer
    // Set CEN (Counter Enable) bit in CR1
    TIM2->CR1 |= TIM_CR1_CEN;
}
