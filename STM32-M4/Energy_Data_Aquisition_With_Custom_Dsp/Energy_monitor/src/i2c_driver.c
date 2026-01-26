/*
 * i2c_driver.c
 * I2C Driver Implementation for STM32F446xx
 */

#include "i2c_driver.h"         // Driver Header
#include "stm32_f446xx.h"       // Device Header

/*
 * @brief  Initializes I2C1 Peripheral
 * @param  None
 * @retval None
 */
// --- I2C ---
void I2C1_Init(void) {
    // 1. Enable Clocks
    ENABLE_GPIOB();             // Enable Clock for GPIOB (SCL/SDA pins)
    ENABLE_I2C1();              // Enable Clock for I2C1 Peripheral

    // 2. Configure GPIO Pins: PB8 (SCL), PB9 (SDA) as Alternate Function Open Drain
    
    // MODER: Set PB8, PB9 to Alternate Function (10)
    // Bits 16-17 (PB8), 18-19 (PB9) -> 10 (2U)
    GPIOB->MODER |= (2U << 16) | (2U << 18);
    
    // OTYPER: Set PB8, PB9 to Open Drain (1)
    // Bits 8, 9 -> 1
    GPIOB->OTYPER |= (1U << 8) | (1U << 9);
    
    // OSPEEDR: Set Speed to High Speed (11)
    // Bits 16-17, 18-19 -> 11 (3U)
    GPIOB->OSPEEDR |= (3U << 16) | (3U << 18);
    
    // AFRH: Set Alternate Function 4 (AF4) for I2C1
    // AF4 = 0100 (4U)
    // PB8 (Bits 0-3), PB9 (Bits 4-7) in AFRH register
    GPIOB->AFRH |= (4U << 0) | (4U << 4);

    // 3. I2C Configuration
    
    // Reset I2C: Set SWRST (Software Reset) bit
    I2C1->CR1 |= (1U << 15); 
    // Clear SWRST bit to release reset
    I2C1->CR1 &= ~(1U << 15);

    // CR2: Set Peripheral Clock Frequency
    // APB1 Clock is 16MHz (default). Set FREQ bits (0-5) to 16.
    I2C1->CR2 = 16; 

    // CCR (Clock Control Register): Set I2C Speed
    // For Standard Mode 100kHz:
    // Thigh + Tlow = 10us.
    // CCR = F_pclk / (2 * F_i2c) = 16MHz / (2 * 100kHz) = 80.
    I2C1->CCR = 80;

    // TRISE (Rise Time Register):
    // Max Rise Time / T_pclk + 1
    // SM Max Rise = 1000ns. T_pclk = 62.5ns.
    // 1000 / 62.5 = 16. + 1 = 17.
    I2C1->TRISE = 17;

    // Enable I2C Peripheral
    // CR1 Bit 0 (PE) = 1
    I2C1->CR1 |= 1;
}

/*
 * @brief  Writes multiple bytes to I2C Device
 * @param  addr: 7-bit Slave Address
 * @param  reg: Register address to start writing to
 * @param  d: Pointer to data buffer
 * @param  c: Count of bytes to write
 * @retval None
 */
void I2C1_WriteMulti(uint8_t addr, uint8_t reg, uint8_t* d, uint16_t c) {
    // Wait until I2C is not busy (Busy flag in SR2, Bit 1)
    while(I2C1->SR2 & 2); 
    
    // Generate START condition
    // CR1 Bit 8 (START) = 1
    I2C1->CR1 |= (1U << 8); 
    
    // Wait for Start Bit (SB) generated (SR1 Bit 0)
    while(!(I2C1->SR1 & 1)); 
    
    // Send 7-bit Address + Write Bit (0)
    // Note: Usually address is shifted left by 1 outside or inside.
    // Here we assume 'addr' is already formatted or we write directly.
    // Standard I2C: Address is Top 7 bits. LSB is R/W.
    // If user passed shifted address in define (0x78), this is correct.
    I2C1->DR = addr; 
    
    // Wait for Address matched (ADDR) flag (SR1 Bit 1)
    while(!(I2C1->SR1 & 2)); 
    
    // Clear ADDR flag: Read SR1 (done in loop check) then Read SR2.
    (void)I2C1->SR2; 
    
    // Wait for Transmit Empty (TXE) (SR1 Bit 7)
    while(!(I2C1->SR1 & (1U << 7))); 
    
    // Send Register Address
    I2C1->DR = reg;
    
    // Loop to send data bytes
    for(uint16_t i=0; i<c; i++) {
        // Wait for TXE (buffer empty)
        while(!(I2C1->SR1 & (1U << 7))); 
        // Send Data Byte
        I2C1->DR = d[i];
    }
    
    // Wait for last byte TXE
    while(!(I2C1->SR1 & (1U << 7))); 
    // Wait for Byte Transfer Finished (BTF) (SR1 Bit 2)
    // This ensures last byte is physically on the bus before STOP.
    while(!(I2C1->SR1 & (1U << 2))); 
    
    // Generate STOP condition
    // CR1 Bit 9 (STOP) = 1
    I2C1->CR1 |= (1U << 9); 
}

/*
 * @brief  Writes a single byte to I2C Device (Wrapper for Multi)
 * @param  addr: 7-bit Slave Address
 * @param  reg: Register address
 * @param  data: Data byte
 * @retval None
 */
void I2C1_Write(uint8_t addr, uint8_t reg, uint8_t data) {
    uint8_t tmp = data;
    I2C1_WriteMulti(addr, reg, &tmp, 1);
}
