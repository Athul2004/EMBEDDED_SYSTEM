/*
 * spi.c
 *
 *  Created on: 29-May-2026
 *      Author: athul
 */


#include "spi.h"

/*
SPI1 Pin Configuration

PA5 -> SPI1_SCK
PA6 -> SPI1_MISO
PA7 -> SPI1_MOSI

PA4 -> BME280 CS (GPIO Output)
*/

void SPI1_Init(void)
{
    /* Enable Clocks */

    GPIOA_CLK_EN();

    SPI1_CLK_EN();

    /*------------------------------------------------
      PA5 -> AF5
      PA6 -> AF5
      PA7 -> AF5
    ------------------------------------------------*/

    GPIOA->MODER &= ~(0x3 << (5 * 2));
    GPIOA->MODER |=  (0x2 << (5 * 2));

    GPIOA->MODER &= ~(0x3 << (6 * 2));
    GPIOA->MODER |=  (0x2 << (6 * 2));

    GPIOA->MODER &= ~(0x3 << (7 * 2));
    GPIOA->MODER |=  (0x2 << (7 * 2));

    /* AFRL AF5 */

    GPIOA->AFRL &= ~(0xF << (5 * 4));
    GPIOA->AFRL |=  (0x5 << (5 * 4));

    GPIOA->AFRL &= ~(0xF << (6 * 4));
    GPIOA->AFRL |=  (0x5 << (6 * 4));

    GPIOA->AFRL &= ~(0xF << (7 * 4));
    GPIOA->AFRL |=  (0x5 << (7 * 4));

    /*---------------------------------------------
      PA4 -> CS Output
    ---------------------------------------------*/

    GPIOA->MODER &= ~(0x3 << (4 * 2));
    GPIOA->MODER |=  (0x1 << (4 * 2));

    /* CS HIGH */

    GPIOA->ODR |= (1 << 4);

    /*---------------------------------------------
      SPI1 Configuration
    ---------------------------------------------*/

    SPI1->CR1 = 0;

    /* Master Mode */

    SPI1->CR1 |= (1 << SPI_CR1_MSTR);

    /* Baud Rate Fpclk/16 */

    SPI1->CR1 |= (0x3 << SPI_CR1_BR);

    /* Software Slave Management */

    SPI1->CR1 |= (1 << SPI_CR1_SSM);

    SPI1->CR1 |= (1 << SPI_CR1_SSI);

    /* SPI Enable */

    SPI1->CR1 |= (1 << SPI_CR1_SPE);
}

/*------------------------------------------------
  Send Data
------------------------------------------------*/

void SPI1_SendData(uint8_t *pTxBuffer, uint32_t Len)
{
    while(Len > 0)
    {
        while(!(SPI1->SR & (1 << SPI_SR_TXE)));

        *((volatile uint8_t*)&SPI1->DR) = *pTxBuffer;

        pTxBuffer++;

        Len--;
    }

    while(SPI1->SR & (1 << SPI_SR_BSY));
}

/*------------------------------------------------
  Receive Data
------------------------------------------------*/

void SPI1_ReceiveData(uint8_t *pRxBuffer, uint32_t Len)
{
    while(Len > 0)
    {
        while(!(SPI1->SR & (1 << SPI_SR_TXE)));

        *((volatile uint8_t*)&SPI1->DR) = 0xFF;

        while(!(SPI1->SR & (1 << SPI_SR_RXNE)));

        *pRxBuffer = *((volatile uint8_t*)&SPI1->DR);

        pRxBuffer++;

        Len--;
    }
}

/*------------------------------------------------
  Full Duplex Transfer
------------------------------------------------*/

uint8_t SPI1_Transfer(uint8_t data)
{
    while(!(SPI1->SR & (1 << SPI_SR_TXE)));

    *((volatile uint8_t*)&SPI1->DR) = data;

    while(!(SPI1->SR & (1 << SPI_SR_RXNE)));

    return *((volatile uint8_t*)&SPI1->DR);
}
