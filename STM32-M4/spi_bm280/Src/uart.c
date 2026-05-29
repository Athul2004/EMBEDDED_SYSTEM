/*
 * uart.c
 *
 *  Created on: 29-May-2026
 *      Author: athul
 */


#include "uart.h"

/*
USART2 TX -> PA2
USART2 RX -> PA3

Baud Rate : 115200
Clock     : 16 MHz HSI
*/

void UART2_Init(void)
{
    /* Enable Clocks */

    GPIOA_CLK_EN();

    USART2_CLK_EN();

    /*-----------------------------------
      PA2 -> USART2_TX (AF7)
      PA3 -> USART2_RX (AF7)
    -----------------------------------*/

    GPIOA->MODER &= ~(0x3 << (2 * 2));
    GPIOA->MODER |=  (0x2 << (2 * 2));

    GPIOA->MODER &= ~(0x3 << (3 * 2));
    GPIOA->MODER |=  (0x2 << (3 * 2));

    /* Alternate Function AF7 */

    GPIOA->AFRL &= ~(0xF << (2 * 4));
    GPIOA->AFRL |=  (0x7 << (2 * 4));

    GPIOA->AFRL &= ~(0xF << (3 * 4));
    GPIOA->AFRL |=  (0x7 << (3 * 4));

    /*-----------------------------------
      Baud Rate = 115200

      Assuming:
      PCLK1 = 16 MHz

      BRR = 0x008B
    -----------------------------------*/

    USART2->BRR = 0x008B;

    /* Enable TX */

    USART2->CR1 |= (1 << USART_CR1_TE);

    /* Enable RX */

    USART2->CR1 |= (1 << USART_CR1_RE);

    /* Enable USART */

    USART2->CR1 |= (1 << USART_CR1_UE);
}

/*-----------------------------------
  Send One Character
-----------------------------------*/

void UART2_SendChar(char ch)
{
    while(!(USART2->SR & (1 << USART_SR_TXE)));

    USART2->DR = ch;
}

/*-----------------------------------
  Send String
-----------------------------------*/

void UART2_SendString(char *str)
{
    while(*str)
    {
        UART2_SendChar(*str++);
    }
}

/*-----------------------------------
  Send Integer Number
-----------------------------------*/

void UART2_SendNumber(uint32_t num)
{
    char buffer[12];

    int i = 0;

    if(num == 0)
    {
        UART2_SendChar('0');
        return;
    }

    while(num > 0)
    {
        buffer[i++] = (num % 10) + '0';

        num /= 10;
    }

    while(i > 0)
    {
        UART2_SendChar(buffer[--i]);
    }
}
