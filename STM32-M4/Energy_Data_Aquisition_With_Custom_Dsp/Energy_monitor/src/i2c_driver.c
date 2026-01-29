/*
 * i2c_driver.c
 * I2C Driver Implementation for STM32F446xx
 * MISRA Refactored
 */

#include "i2c_driver.h"

// Timeout value for I2C loops to prevent hanging
#define I2C_TIMEOUT  100000U

/*
 * @brief  Initializes I2C1 Peripheral
 * @param  None
 * @retval None
 */
void I2C1_Init(void) {
    // 1. Enable Clocks
    ENABLE_GPIOB();             // Enable Clock for GPIOB (SCL/SDA pins)
    ENABLE_I2C1();              // Enable Clock for I2C1 Peripheral

    // 2. Configure GPIO Pins: PB8 (SCL), PB9 (SDA) as Alternate Function Open Drain
    
    // MODER: Set PB8, PB9 to Alternate Function (10 = 2U)
    GPIOB->MODER |= (2U << 16) | (2U << 18);
    
    // OTYPER: Set PB8, PB9 to Open Drain (1)
    GPIOB->OTYPER |= (1U << 8) | (1U << 9);
    
    // OSPEEDR: Set Speed to High Speed (11 = 3U)
    GPIOB->OSPEEDR |= (3U << 16) | (3U << 18);
    
    // AFRH: Set Alternate Function 4 (AF4 = 0100 = 4U) for I2C1
    // PB8 (Bits 0-3), PB9 (Bits 4-7) in AFRH register
    GPIOB->AFRH |= (4U << 0) | (4U << 4);

    // 3. I2C Configuration
    
    // Reset I2C: Set SWRST (Software Reset) bit
    I2C1->CR1 |= I2C_CR1_SWRST;
    // Clear SWRST bit to release reset
    I2C1->CR1 &= ~I2C_CR1_SWRST;

    // CR2: Set Peripheral Clock Frequency
    // APB1 Clock is 16MHz (default). Set FREQ bits (0-5) to 16.
    I2C1->CR2 = 16U; 

    // CCR (Clock Control Register): Set I2C Speed
    // For Standard Mode 100kHz:
    // Thigh + Tlow = 10us.
    // CCR = F_pclk / (2 * F_i2c) = 16MHz / (2 * 100kHz) = 80.
    I2C1->CCR = 80U;

    // TRISE (Rise Time Register):
    // Max Rise Time / T_pclk + 1
    // SM Max Rise = 1000ns. T_pclk = 62.5ns (1/16MHz).
    // 1000 / 62.5 = 16. + 1 = 17.
    I2C1->TRISE = 17U;

    // Enable I2C Peripheral
    I2C1->CR1 |= I2C_CR1_PE;
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
    volatile uint32_t timeout;

    // Wait until I2C is not busy
    timeout = I2C_TIMEOUT;
    while((I2C1->SR2 & I2C_SR2_BUSY) && (timeout > 0U)){ timeout--; }
    if(timeout == 0U) return; // Error or Bus Busy stuck

    // Generate START condition
    I2C1->CR1 |= I2C_CR1_START;
    
    // Wait for Start Bit (SB) generated
    timeout = I2C_TIMEOUT;
    while(!(I2C1->SR1 & I2C_SR1_SB) && (timeout > 0U)){ timeout--; }
    if(timeout == 0U) return;
    
    // Send 7-bit Address
    // Standard I2C: Address is Top 7 bits. LSB is 0 for Write.
    I2C1->DR = addr; 
    
    // Wait for Address matched (ADDR) flag
    timeout = I2C_TIMEOUT;
    while(!(I2C1->SR1 & I2C_SR1_ADDR) && (timeout > 0U)){ timeout--; }
    if(timeout == 0U) return;
    
    // Clear ADDR flag: Read SR1 (done in loop check) then Read SR2.
    (void)I2C1->SR2; 
    
    // Wait for Transmit Empty (TXE)
    timeout = I2C_TIMEOUT;
    while(!(I2C1->SR1 & I2C_SR1_TXE) && (timeout > 0U)){ timeout--; }
    
    // Send Register Address
    I2C1->DR = reg;
    
    // Loop to send data bytes
    for(uint16_t i=0; i<c; i++) {
        // Wait for TXE (buffer empty)
        timeout = I2C_TIMEOUT;
        while(!(I2C1->SR1 & I2C_SR1_TXE) && (timeout > 0U)){ timeout--; }
        
        // Send Data Byte
        I2C1->DR = d[i];
    }
    
    // Wait for last byte TXE
    timeout = I2C_TIMEOUT;
    while(!(I2C1->SR1 & I2C_SR1_TXE) && (timeout > 0U)){ timeout--; }
    
    // Wait for Byte Transfer Finished (BTF)
    // This ensures last byte is physically on the bus before STOP.
    timeout = I2C_TIMEOUT;
    while(!(I2C1->SR1 & I2C_SR1_BTF) && (timeout > 0U)){ timeout--; }
    
    // Generate STOP condition
    I2C1->CR1 |= I2C_CR1_STOP; 
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

// Stubs for Generic APIs if needed by other modules
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi) {}
void I2C_Init(I2C_Handle_t *pI2CHandle) {}
void I2C_DeInit(I2C_RegDef_t *pI2Cx) {}
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr) {}
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr) {}
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx , uint32_t FlagName) { return 0; }
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t AppEv) {}
