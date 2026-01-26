/*
 * adc_dma_driver.c
 * ADC1 + DMA2 Configuration
 */

#include "adc_dma_driver.h"
#include "stm32_f446xx.h"

void ADC_DMA_Init(uint32_t *buffer, uint32_t length) {
    ENABLE_GPIOA();
    ENABLE_ADC1();
    ENABLE_DMA2();

    // GPIO - PA0, PA1 Analog
    // Snippet: GPIOA->MODER |= (3 << 0) | (3 << 2);
    GPIOA->MODER |= (3U << 0) | (3U << 2);

    // ADC1 Config
    // CR1: SCAN (Bit 8)
    ADC1->CR1 |= (1U << 8);

    // CR2: DMA=1, DDS=1, EXTEN=01 (Rising), EXTSEL=0110 (TIM2_TRGO)
    // Snippet: ADC1->CR2 |= (1 << 8) | (1 << 9) | (6 << 24) | (1 << 28);
    ADC1->CR2 |= (1U << 8) | (1U << 9) | (6U << 24) | (1U << 28);

    // SQR1: L=1 (2 conversions) -> Snippet: ADC1->SQR1 = (1 << 20);
    // Be careful, SQR1 direct assignment overwrites other bits. 
    // Snippet does `ADC1->SQR1 = ...`, assuming reset state? or just lazy.
    // I should probably follow snippet exactly if "without arror" is key.
    // But direct assignment is safer if we want exact state.
    // I will use direct assignment as per snippet.
    ADC1->SQR1 = (1U << 20);

    // SQR3: Ch0 (SQ1), Ch1 (SQ2) -> Snippet: ADC1->SQR3 = (0 << 0) | (1 << 5);
    ADC1->SQR3 = (0U << 0) | (1U << 5);

    // ADON (Enable)
    // Snippet: ADC1->CR2 |= 1;
    ADC1->CR2 |= (1U << 0);

    // DMA2 Stream 0 Config
    // Snippet: DMA2_Stream0->CR &= ~1; while(..); 
    DMA2_Stream0->CR &= ~(1U << 0); // Disable
    while(DMA2_Stream0->CR & (1U << 0)); // Wait

    // Snippet: PAR, M0AR, NDTR
    DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;
    DMA2_Stream0->M0AR = (uint32_t)buffer;
    DMA2_Stream0->NDTR = length;

    // Snippet: CR = (0 << 25) | (2 << 13) | (2 << 11) | (1 << 10) | (1 << 8);
    // Chan 0, MSIZE Word, PSIZE Word, MINC, CIRC
    DMA2_Stream0->CR = (0U << 25) | (2U << 13) | (2U << 11) | (1U << 10) | (1U << 8);

    // Enable
    // Snippet: DMA2_Stream0->CR |= 1;
    DMA2_Stream0->CR |= (1U << 0);
}
