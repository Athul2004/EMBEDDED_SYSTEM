/*
 * adc_dma_driver.h
 * ADC1 + DMA2 Driver Header
 */

#ifndef ADC_DMA_DRIVER_H_
#define ADC_DMA_DRIVER_H_

#include "stm32_f446xx.h"    // Include hardware definitions

/*
 * =========================================================================================
 *                                     ADC/DMA CONFIGURATION MACROS
 * =========================================================================================
 */

// GPIO Mode Definitions (MODER)
#define GPIO_MODE_INPUT     0x00U       // Input mode configuration value
#define GPIO_MODE_OUTPUT    0x01U       // Output mode configuration value
#define GPIO_MODE_ALTFN     0x02U       // Alternate function mode configuration value
#define GPIO_MODE_ANALOG    0x03U       // Analog mode configuration value

// ADC Control Register 1 (CR1)
#define ADC_CR1_SCAN        (1U << 8)   // Scan mode enable bit (Bit 8). Scans all channels.

// ADC Control Register 2 (CR2)
#define ADC_CR2_ADON        (1U << 0)   // A/D Converter ON / OFF bit (Bit 0)
#define ADC_CR2_DMA         (1U << 8)   // Direct Memory Access mode enable bit (Bit 8)
#define ADC_CR2_DDS         (1U << 9)   // DMA disable selection bit (Bit 9). 1=DMA requests continue.
#define ADC_CR2_EXTEN_RISING (1U << 28) // External trigger enable: Rising edge (Bits 28-29 -> 01)
#define ADC_CR2_EXTSEL_TIM2_TRGO (0x6U << 24) // External event select: TIM2_TRGO (Bits 24-27 -> 0110)

// ADC Regular Sequence Register 1 (SQR1)
#define ADC_SQR1_L_2CONV    (1U << 20)  // Regular channel sequence length: 2 conversions (Bits 20-23 -> 0001)

// DMA Streams
#define DMA_STREAM_EN       (1U << 0)   // Stream Enable bit (Bit 0)

// API Function Prototypes

// Initializes ADC1 and DMA2 with the specified buffer
void ADC_DMA_Init(uint32_t *buffer, uint32_t length);

#endif /* ADC_DMA_DRIVER_H_ */
