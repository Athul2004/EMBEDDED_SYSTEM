/*
 * adc_dma_driver.c
 * ADC1 + DMA2 Configuration Implementation
 */

#include "adc_dma_driver.h"     // Driver Header
#include "stm32_f446xx.h"       // Device Header

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
    // 3U = 0b11. Shifted by 0 for PA0, by 2 for PA1.
    GPIOA->MODER |= (3U << 0) | (3U << 2);

    // 3. Configure ADC1 Settings
    
    // CR1 (Control Register 1): Enable SCAN Mode
    // Bit 8 (SCAN) = 1. Allows scanning multiple channels (Ch0, Ch1)
    ADC1->CR1 |= (1U << 8);

    // CR2 (Control Register 2): DMA and Trigger Configuration
    // Bit 8 (DMA) = 1: Enable DMA Request generation
    // Bit 9 (DDS) = 1: DMA Disable Selection (1 = DMA requests continue after conversion)
    // Bits 24-27 (EXTSEL) = 6 (0110): External Event Select -> TIM2_TRGO (Timer 2 Trigger Output)
    // Bits 28-29 (EXTEN) = 1 (01): External Trigger Enable (Trigger detection on Rising Edge)
    ADC1->CR2 |= (1U << 8) | (1U << 9) | (6U << 24) | (1U << 28);

    // SQR1 (Regular Sequence Register 1): Sequence Length
    // Bits 20-23 (L): Total number of conversions - 1
    // We want 2 conversions (Ch0, Ch1). L = 2 - 1 = 1.
    // Clear bits (0xF) then set to 1.
    ADC1->SQR1 &= ~(0xFU << 20); // Clear L bits
    ADC1->SQR1 |= (1U << 20);    // Set L = 1

    // SQR3 (Regular Sequence Register 3): Channel Selection
    // SQ1 (Bits 0-4): 1st conversion in sequence -> Channel 0 (PA0)
    // SQ2 (Bits 5-9): 2nd conversion in sequence -> Channel 1 (PA1)
    // Clear bits first then set.
    ADC1->SQR3 &= ~((0x1FU << 0) | (0x1FU << 5)); // Clear SQ1, SQ2
    ADC1->SQR3 |= (0U << 0) | (1U << 5);          // Set SQ1=0, SQ2=1

    // Enable ADC Peripheral
    // CR2 Bit 0 (ADON) = 1
    ADC1->CR2 |= (1U << 0);

    // 4. Configure DMA2 Stream 0 (ADC1 is mapped to Stream 0, Channel 0)
    
    // Disable Stream before configuration
    // CR (Control Register) Bit 0 (EN) = 0
    DMA2_Stream0->CR &= ~(1U << 0);
    // Wait until EN bit is cleared (hardware acknowledgement)
    while(DMA2_Stream0->CR & (1U << 0));

    // Configure Addresses
    // PAR (Peripheral Address Register): Source address -> ADC1 Data Register (DR)
    DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;
    // M0AR (Memory 0 Address Register): Destination address -> User Buffer
    DMA2_Stream0->M0AR = (uint32_t)buffer;
    // NDTR (Number of Data Register): Number of data items to transfer
    DMA2_Stream0->NDTR = length;

    // Configure Stream Control Register (CR)
    // Channel Select (Bits 25-27): Channel 0 (000) for ADC1
    // MSIZE (Bits 13-14): Memory Data Size -> Word (32-bit) = 10 (2U)
    // PSIZE (Bits 11-12): Peripheral Data Size -> Word (32-bit) = 10 (2U)
    // MINC (Bit 10): Memory Increment Mode -> Enable (1). Buf ptr increments after transfer.
    // CIRC (Bit 8): Circular Mode -> Enable (1). Buffer wraps around continuously.
    // DIR (Bits 6-7): Data Transfer Direction -> Peripheral to Memory (00)
    // PL (Bits 16-17): Priority Level -> Very High (11) - Optional but good practice
    // TEIE/DMEIE not enabled, relying on Flag Polling in App.
    
    // Constructing CR value:
    // (0 << 25) | (2 << 13) | (2 << 11) | (1 << 10) | (1 << 8)
    DMA2_Stream0->CR = (0U << 25) | (2U << 13) | (2U << 11) | (1U << 10) | (1U << 8);

    // Enable DMA Stream
    // CR Bit 0 (EN) = 1
    DMA2_Stream0->CR |= (1U << 0);
}
