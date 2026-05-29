#include "stm32f44xx.h"

void delay_ms(uint32_t ms)
{
    for(uint32_t i = 0; i < ms; i++)
    {
        for(uint32_t j = 0; j < 1600; j++);
    }
}

void UART2_Init(void)
{
    // Enable GPIOA clock
    GPIOA_CLK_EN();

    // Enable USART2 clock
    USART2_EN();

    // PA2 -> Alternate Function Mode
    GPIOA->MODER &= ~(0x3 << (2 * 2));
    GPIOA->MODER |=  (0x2 << (2 * 2));

    // Select AF7 (USART2_TX) for PA2
    GPIOA->AFRL &= ~(0xF << (2 * 4));
    GPIOA->AFRL |=  (0x7 << (2 * 4));

    // Baud Rate = 115200
    // Assuming APB1 clock = 16 MHz
    USART2->BRR = 0x008B;

    // Enable Transmitter (TE)
    USART2->CR1 |= (1 << 3);

    // Enable USART (UE)
    USART2->CR1 |= (1 << 13);
}

void UART2_SendChar(char ch)
{
    while(!(USART2->SR & (1 << 7)));

    USART2->DR = ch;
}

void UART2_SendString(char *str)
{
    while(*str)
    {
        UART2_SendChar(*str++);
    }
}

int main(void)
{
    // LED GPIO Clock
    GPIOA_CLK_EN();

    // PA5 Output Mode
    GPIOA->MODER &= ~(0x3 << (5 * 2));
    GPIOA->MODER |=  (0x1 << (5 * 2));

    // Initialize UART2
    UART2_Init();

    while(1)
    {
        // LED ON
        GPIOA->ODR |= (1 << 5);

        UART2_SendString("LED ON\r\n");

        delay_ms(1000);

        // LED OFF
        GPIOA->ODR &= ~(1 << 5);

        UART2_SendString("LED OFF\r\n");

        delay_ms(1000);
    }
}
