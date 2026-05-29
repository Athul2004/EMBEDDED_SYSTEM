/*
 * i2c.c
 *
 *  Created on: 30-May-2026
 *      Author: athul
 */


#include "i2c.h"

void I2C1_Init(void)
{
    /* Enable GPIOB Clock */
    RCC->AHB1ENR |= (1 << 1);

    /* Enable I2C1 Clock */
    RCC->APB1ENR |= (1 << 21);

    /* PB8 = SCL, PB9 = SDA */

    GPIOB->MODER &= ~(3 << 16);
    GPIOB->MODER &= ~(3 << 18);

    GPIOB->MODER |= (2 << 16);
    GPIOB->MODER |= (2 << 18);

    GPIOB->OTYPER |= (1 << 8);
    GPIOB->OTYPER |= (1 << 9);

    GPIOB->PUPDR &= ~(3 << 16);
    GPIOB->PUPDR &= ~(3 << 18);

    GPIOB->PUPDR |= (1 << 16);
    GPIOB->PUPDR |= (1 << 18);

    GPIOB->AFR[1] &= ~(0xF << 0);
    GPIOB->AFR[1] &= ~(0xF << 4);

    GPIOB->AFR[1] |= (4 << 0);
    GPIOB->AFR[1] |= (4 << 4);

    /* Reset I2C */
    I2C1->CR1 = 0x0000;

    /* APB1 Clock = 16 MHz */
    I2C1->CR2 = 16;

    /* 100 kHz I2C */
    I2C1->CCR = 80;

    /* Maximum rise time */
    I2C1->TRISE = 17;

    /* Enable I2C */
    I2C1->CR1 |= (1 << 0);
}

void I2C1_Write(uint8_t slave_addr,
                uint8_t reg_addr,
                uint8_t data)
{
    /* START */
    I2C1->CR1 |= (1 << 8);

    while(!(I2C1->SR1 & (1 << 0)));

    /* Slave Address + Write */
    I2C1->DR = slave_addr;

    while(!(I2C1->SR1 & (1 << 1)));

    (void)I2C1->SR2;

    /* Register Address */
    while(!(I2C1->SR1 & (1 << 7)));

    I2C1->DR = reg_addr;

    while(!(I2C1->SR1 & (1 << 7)));

    /* Data */
    I2C1->DR = data;

    while(!(I2C1->SR1 & (1 << 7)));

    /* STOP */
    I2C1->CR1 |= (1 << 9);
}

void I2C1_Read(uint8_t slave_addr,
               uint8_t reg_addr,
               uint8_t *data)
{
    /* START */
    I2C1->CR1 |= (1 << 8);

    while(!(I2C1->SR1 & (1 << 0)));

    /* Slave Address + Write */
    I2C1->DR = slave_addr;

    while(!(I2C1->SR1 & (1 << 1)));

    (void)I2C1->SR2;

    while(!(I2C1->SR1 & (1 << 7)));

    /* Register Address */
    I2C1->DR = reg_addr;

    while(!(I2C1->SR1 & (1 << 7)));

    /* Repeated START */
    I2C1->CR1 |= (1 << 8);

    while(!(I2C1->SR1 & (1 << 0)));

    /* Slave Address + Read */
    I2C1->DR = slave_addr | 0x01;

    while(!(I2C1->SR1 & (1 << 1)));

    /* NACK for single byte */
    I2C1->CR1 &= ~(1 << 10);

    (void)I2C1->SR2;

    /* STOP */
    I2C1->CR1 |= (1 << 9);

    while(!(I2C1->SR1 & (1 << 6)));

    *data = I2C1->DR;
}
