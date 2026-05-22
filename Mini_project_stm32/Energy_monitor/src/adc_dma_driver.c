/*
 * adc_dma_driver.c
 * ADC1 + DMA2 Configuration Implementation
 */

#include "adc_dma_driver.h" // Include driver header definition

/*
 * @brief  Initializes ADC1 and DMA2 for Continuous Scan Mode with Timer Trigger
 * @param  buffer: Pointer to memory buffer where ADC data will be stored
 * @param  length: Size of the buffer (number of items)
 * @retval None
 */
void ADC_DMA_Init(uint32_t *buffer, uint32_t length) {
    // 1. Enable Peripheral Clocks
    ENABLE_GPIOA();     // Enable Clock for GPIO Port A (Pins PA0, PA1) by setting RCC AHB1ENR bit
    ENABLE_ADC1();      // Enable Clock for ADC1 Peripheral by setting RCC APB2ENR bit
    ENABLE_DMA2();      // Enable Clock for DMA2 Peripheral (ADC1 is on DMA2) by setting RCC AHB1ENR bit

    // 2. Configure GPIO Pins (PA0, PA1) as Analog Mode
    // MODER Register: Set bits 0-1 (PA0) and 2-3 (PA1) to '11' (Binary 3) which is Analog Mode
    GPIOA->MODER |= (GPIO_MODE_ANALOG << 0) | (GPIO_MODE_ANALOG << 2);

    // 3. Configure ADC1 Settings
    
    // CR1 (Control Register 1): Enable SCAN Mode
    // Scan mode converts channels in a group one after another
    ADC1->CR1 |= ADC_CR1_SCAN;

    // CR2 (Control Register 2): DMA and Trigger Configuration
    // ADC_CR2_DMA: Enable Direct Memory Access mode, ADC requests DMA transfer after conversion
    // ADC_CR2_DDS: DMA disable selection. 1 = DMA requests continue forever (Circular buffer support)
    // ADC_CR2_EXTSEL_TIM2_TRGO: Select External Event 6 (TIM2_TRGO) (Bits 24-27 = 0110)
    // ADC_CR2_EXTEN_RISING: Enable External Trigger on Rising Edge (Bits 28-29 = 01)
    ADC1->CR2 |= (ADC_CR2_DMA | ADC_CR2_DDS | ADC_CR2_EXTSEL_TIM2_TRGO | ADC_CR2_EXTEN_RISING);

    // SQR1 (Regular Sequence Register 1): Sequence Length
    // Clear L bits (20-23) first to reset length configuration
    ADC1->SQR1 &= ~(0xFU << 20); 
    // Set L = 1 (2 conversions) because L is (Count - 1). So 1 means 2 channels.
    ADC1->SQR1 |= ADC_SQR1_L_2CONV;

    // SQR3 (Regular Sequence Register 3): Channel Selection
    // SQ1 (Bits 0-4) is the 1st conversion in sequence.
    // SQ2 (Bits 5-9) is the 2nd conversion in sequence.
    // We want Channel 0 first, then Channel 1.
    ADC1->SQR3 &= ~((0x1FU << 0) | (0x1FU << 5)); // Clear SQ1 and SQ2 fields
    ADC1->SQR3 |= (0U << 0) | (1U << 5);          // Set SQ1=0 (Ch0), SQ2=1 (Ch1)

    // Enable ADC Peripheral by setting ADON bit in CR2
    ADC1->CR2 |= ADC_CR2_ADON;

    // 4. Configure DMA2 Stream 0 (ADC1 is mapped to Stream 0, Channel 0)
    
    // Disable Stream 0 before configuration by clearing EN bit in CR
    DMA2_Stream0->CR &= ~DMA_STREAM_EN;
    
    // Wait until EN bit is cleared (hardware acknowledgement)
    // This ensures no active transfer is interrupted and stream is ready
    while((DMA2_Stream0->CR & DMA_STREAM_EN) != 0U);

    // Configure Addresses
    // PAR: Peripheral Address Register. Set to ADC1 Data Register (DR) address
    DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;
    // M0AR: Memory 0 Address Register. Set to the user provided buffer address
    DMA2_Stream0->M0AR = (uint32_t)buffer;
    // NDTR: Number of Data Items to Transfer. Set to buffer length.
    DMA2_Stream0->NDTR = length;

    // Configure Stream Control Register (CR)
    // Channel Selection (CHSEL): Channel 0 is 000 (Bits 25-27)
    // Priority Level (PL): Very High is 11 (3) (Bits 16-17)
    // Memory Data Size (MSIZE): 32-bit is 10 (2) (Bits 13-14)
    // Peripheral Data Size (PSIZE): 32-bit is 10 (2) (Bits 11-12)
    // Memory Increment Mode (MINC): Enabled is 1 (Bit 10) - increment memory pointer
    // Circular Mode (CIRC): Enabled is 1 (Bit 8) - buffer wraps around
    // Data Transfer Direction (DIR): Peripheral to Memory is 00 (Bits 6-7)
    // The following expression combines these flags:
    DMA2_Stream0->CR = (0U << 25) | (3U << 16) | (2U << 13) | (2U << 11) | (1U << 10) | (1U << 8);

    // Enable DMA Stream by setting EN bit in CR
    DMA2_Stream0->CR |= DMA_STREAM_EN;
}
