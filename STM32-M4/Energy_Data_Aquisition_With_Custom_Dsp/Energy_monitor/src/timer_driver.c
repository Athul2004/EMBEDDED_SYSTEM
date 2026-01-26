/*
 * timer_driver.c
 * TIM2 Configuration Implementation
 */

#include "timer_driver.h"       // Driver Header
#include "stm32_f446xx.h"       // Device Header

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
    // F_update = F_clk / ((PSC+1) * (ARR+1))
    // 8000 = 16000000 / (1 * 2000)
    
    // PSC (Prescaler): Divide clock by PSC+1. Set to 0 (Divide by 1).
    TIM2->PSC = 0;
    
    // ARR (Auto-Reload Register): Counter counts up to ARR then resets. 
    // Set to 1999 (2000 counts).
    TIM2->ARR = 1999;
    
    // 3. Configure Trigger Output (TRGO)
    // CR2 (Control Register 2) MMS Bits (Master Mode Selection)
    // We want TRGO signal to pulse on Update Event (UEV - Counter Overflow)
    // MMS = 010 (Update) -> Value 2 (shifted to bits 4-6)
    
    // Clear MMS bits (4-6) first
    TIM2->CR2 &= ~(7U << 4);
    // Set MMS bits to 010 (2)
    TIM2->CR2 |= (2U << 4);
    
    // 4. Enable Timer
    // CR1 (Control Register 1) Bit 0 (CEN) = 1
    TIM2->CR1 |= 1;
}
