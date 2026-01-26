/*
 * i2c_driver.c
 * I2C Driver Implementation for STM32F446xx
 */

#include "i2c_driver.h"
#include "stm32_f446xx.h"

// --- I2C ---
void I2C1_Init(void) {
    // 1. Enable Clocks
    ENABLE_GPIOB();
    ENABLE_I2C1();

    // 2. GPIO Pins: PB8 (SCL), PB9 (SDA) -> AF4
    // Config from snippet:
    // GPIOB->MODER |= (2 << 16) | (2 << 18);
    // GPIOB->OTYPER |= (1 << 8) | (1 << 9);
    // GPIOB->OSPEEDR |= (3 << 16) | (3 << 18);
    // GPIOB->AFRH |= (4 << 0) | (4 << 4);
    
    GPIOB->MODER |= (2U << 16) | (2U << 18);
    GPIOB->OTYPER |= (1U << 8) | (1U << 9);
    GPIOB->OSPEEDR |= (3U << 16) | (3U << 18);
    GPIOB->AFRH |= (4U << 0) | (4U << 4);

    // 3. I2C Config
    I2C1->CR1 |= (1U << 15); // SWRST
    I2C1->CR1 &= ~(1U << 15);

    I2C1->CR2 = 16;
    I2C1->CCR = 80;
    I2C1->TRISE = 17;
    I2C1->CR1 |= 1;
}

void I2C1_WriteMulti(uint8_t addr, uint8_t reg, uint8_t* d, uint16_t c) {
    while(I2C1->SR2 & 2); // Wait Busy
    
    I2C1->CR1 |= (1U << 8); // Start
    while(!(I2C1->SR1 & 1)); // Wait SB
    
    I2C1->DR = addr; // Address
    while(!(I2C1->SR1 & 2)); // Wait ADDR
    (void)I2C1->SR2; // Clear ADDR
    
    // Snippet: while(!(I2C1->SR1 & (1<<7))); I2C1->DR = reg;
    while(!(I2C1->SR1 & (1U << 7))); // Wait TXE
    I2C1->DR = reg;
    
    for(uint16_t i=0; i<c; i++) {
        while(!(I2C1->SR1 & (1U << 7)));
        I2C1->DR = d[i];
    }
    
    while(!(I2C1->SR1 & (1U << 7))); // Wait TXE
    while(!(I2C1->SR1 & (1U << 2))); // Wait BTF (Corrected from snippet `while(SR1 & 2)` which likely meant `while(!BTF)`)
    
    I2C1->CR1 |= (1U << 9); // Stop
}

void I2C1_Write(uint8_t addr, uint8_t reg, uint8_t data) {
    uint8_t tmp = data;
    I2C1_WriteMulti(addr, reg, &tmp, 1);
}
