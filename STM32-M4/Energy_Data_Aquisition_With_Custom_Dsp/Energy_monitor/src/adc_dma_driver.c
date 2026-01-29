/*
 * adc_dma_driver.c
 * ADC1 + DMA2 Configuration Implementation
 */

#include "adc_dma_driver.h"

/*
 * @brief  Initializes ADC1 and DMA2 for Continuous Scan Mode with Timer Trigger
 * @param  buffer: Pointer to memory buffer where ADC data will be stored
 * @param  length: Size of the buffer (number of items)
 * @retval None
 */
void ADC_DMA_Init(uint32_t *buffer, uint32_t length) {
    // 1. Enable Peripheral Clocks
    ENABLE_GPIOA();     // Enable Clock for GPIO Port A (Pins PA0, PA1)
    ENABLE_ADC1();      // Enable Clock for ADC1 Peripheral
    ENABLE_DMA2();      // Enable Clock for DMA2 Peripheral (ADC1 is on DMA2)

    // 2. Configure GPIO Pins (PA0, PA1) as Analog Mode
    // MODER Register: Set bits 0-1 and 2-3 to '11' (Analog)
    GPIOA->MODER |= (GPIO_MODE_ANALOG << 0) | (GPIO_MODE_ANALOG << 2);

    // 3. Configure ADC1 Settings
    
    // CR1 (Control Register 1): Enable SCAN Mode
    ADC1->CR1 |= ADC_CR1_SCAN;

    // CR2 (Control Register 2): DMA and Trigger Configuration
    // Enable DMA, DDS (DMA Requests continue), External Trigger on Rising Edge, TIM2_TRGO
    ADC1->CR2 |= (ADC_CR2_DMA | ADC_CR2_DDS | ADC_CR2_EXTSEL_TIM2_TRGO | ADC_CR2_EXTEN_RISING);

    // SQR1 (Regular Sequence Register 1): Sequence Length
    // Clear L bits (20-23) first
    ADC1->SQR1 &= ~(0xFU << 20); 
    // Set L = 1 (2 conversions)
    ADC1->SQR1 |= ADC_SQR1_L_2CONV;

    // SQR3 (Regular Sequence Register 3): Channel Selection
    // SQ1 (Bits 0-4) -> Ch0, SQ2 (Bits 5-9) -> Ch1
    ADC1->SQR3 &= ~((0x1FU << 0) | (0x1FU << 5)); // Clear SQ1, SQ2
    ADC1->SQR3 |= (0U << 0) | (1U << 5);          // Set SQ1=0, SQ2=1

    // Enable ADC Peripheral
    ADC1->CR2 |= ADC_CR2_ADON;

    // 4. Configure DMA2 Stream 0 (ADC1 is mapped to Stream 0, Channel 0)
    
    // Disable Stream before configuration
    DMA2_Stream0->CR &= ~DMA_STREAM_EN;
    
    // Wait until EN bit is cleared (hardware acknowledgement)
    while((DMA2_Stream0->CR & DMA_STREAM_EN) != 0U);

    // Configure Addresses
    DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;
    DMA2_Stream0->M0AR = (uint32_t)buffer;
    DMA2_Stream0->NDTR = length;

    // Configure Stream Control Register (CR)
    // Channel 0 (000), Word Size (32-bit), Memory Inc (Enable), Circular Mode (Enable), Dir (P2M)
    // CR Value construction:
    // CHSEL = 0 (Bits 25-27)
    // MSIZE = 2 (Bits 13-14) -> 32-bit
    // PSIZE = 2 (Bits 11-12) -> 32-bit
    // MINC  = 1 (Bit 10)
    // CIRC  = 1 (Bit 8)
    // DIR   = 0 (Bits 6-7) -> Peripheral to Memory
    // PL    = 3 (Bits 16-17) -> Very High Priority
    DMA2_Stream0->CR = (0U << 25) | (3U << 16) | (2U << 13) | (2U << 11) | (1U << 10) | (1U << 8);

    // Enable DMA Stream
    DMA2_Stream0->CR |= DMA_STREAM_EN;
}
