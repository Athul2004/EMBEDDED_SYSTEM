/*
 * uart_driver.c
 * UART Driver Implementation for STM32F446xx
 */

#include "uart_driver.h"
#include "stm32_f446xx.h"

// --- UART ---
void UART2_Init(void) {
    ENABLE_GPIOA();
    ENABLE_UART2();

    // Configure PA2, PA3 with logic from snippet:
    // GPIOA->MODER |= (3 << 0) | (3 << 2); // This was PA0/PA1 in Hardware_Init, ignore.
    // GPIOA->MODER |= (2 << 4) | (2 << 6); // PA2, PA3 Alternate Function
    // GPIOA->AFRL  |= (7 << 8) | (7 << 12); // AF7
    
    // Clear and set MODER for PA2, PA3 (Bits 4-7)
    // Snippet assumes default 00? Or just ORs? Snippet: GPIOA->MODER |= (2 << 4) | (2 << 6);
    // Best practice is clear then set, but snippet logic is just |=.
    // I will use |= as requested, but to be safe I'll clear first if likely.
    // Wait, user said "add this code... without arror".
    // I will replicate snippet logic exactly: |=
    
    GPIOA->MODER |= (2U << 4) | (2U << 6);
    GPIOA->AFRL  |= (7U << 8) | (7U << 12);

    // USART2->BRR = 0x008B;
    USART2->BRR = 0x008B;
    
    // USART2->CR1 = (1 << 13) | (1 << 3) | (1 << 2);
    USART2->CR1 = (1U << 13) | (1U << 3) | (1U << 2);
}

void UART2_SendString(char *string) {
    while(*string) {
        while (!(USART2->SR & (1U << 7)));
        USART2->DR = *string++;
    }
}

void UART2_SendNumber(int number) {
    char buf[12]; 
    int i=0;
    if(number==0) { UART2_SendString("0"); return; }
    if(number<0) { UART2_SendString("-"); number=-number; }
    while(number>0) { buf[i++]=(number%10)+'0'; number/=10; }
    while(--i>=0) { 
        while(!(USART2->SR & (1U << 7))); 
        USART2->DR=buf[i]; 
    }
}

char UART2_GetChar(void) {
    while (!(USART2->SR & (1U << 5)));
    return (char)USART2->DR;
}
