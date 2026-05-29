/*
 * uart.c
 *
 *  Created on: 30-May-2026
 *      Author: athul
 */


#include "uart.h"

void UART2_Init(void)
{
    /* Enable GPIOA Clock */
    RCC->AHB1ENR |= (1 << 0);

    /* Enable USART2 Clock */
    RCC->APB1ENR |= (1 << 17);

    /* PA2 -> USART2_TX */

    GPIOA->MODER &= ~(3 << 4);
    GPIOA->MODER |=  (2 << 4);

    GPIOA->AFR[0] &= ~(0xF << 8);
    GPIOA->AFR[0] |=  (7 << 8);

    /* 9600 baud @ 16 MHz */
    USART2->BRR = 0x0683;

    /* Enable TX */
    USART2->CR1 |= (1 << 3);

    /* Enable USART */
    USART2->CR1 |= (1 << 13);
}

void UART2_WriteChar(char c)
{
    while(!(USART2->SR & (1 << 7)));

    USART2->DR = c;
}

void UART2_WriteString(char *str)
{
    while(*str)
    {
        UART2_WriteChar(*str++);
    }
}
