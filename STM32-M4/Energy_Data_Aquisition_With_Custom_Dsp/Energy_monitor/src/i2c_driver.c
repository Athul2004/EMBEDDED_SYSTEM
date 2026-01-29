/*
 * i2c_driver.c
 * I2C Driver Implementation for STM32F446xx
 * MISRA Refactored
 */

#include "i2c_driver.h" // Include I2C driver header

// Timeout value for I2C loops to prevent hanging
// 100000 iterations is sufficient for most operations at standard system speeds
#define I2C_TIMEOUT  100000U

/*
 * @brief  Initializes I2C1 Peripheral
 * @param  None
 * @retval None
 */
void I2C1_Init(void) {
    // 1. Enable Clocks
    ENABLE_GPIOB();             // Enable Clock for GPIOB (SCL/SDA pins are on PB8/PB9)
    ENABLE_I2C1();              // Enable Clock for I2C1 Peripheral

    // 2. Configure GPIO Pins: PB8 (SCL), PB9 (SDA) as Alternate Function Open Drain
    
    // MODER: Set PB8, PB9 to Alternate Function (10 in binary = 2U)
    // Shift by 16 for Pin 8, 18 for Pin 9
    GPIOB->MODER |= (2U << 16) | (2U << 18);
    
    // OTYPER: Set PB8, PB9 to Open Drain (1)
    // Open Drain is required for I2C bus because lines are pulled up externally
    GPIOB->OTYPER |= (1U << 8) | (1U << 9);
    
    // OSPEEDR: Set Speed to High Speed (11 in binary = 3U)
    // Faster slew rate for cleaner edges
    GPIOB->OSPEEDR |= (3U << 16) | (3U << 18);
    
    // AFRH: Set Alternate Function 4 (AF4 = 0100 = 4U) for I2C1
    // PB8 (Bits 0-3 in AFRH), PB9 (Bits 4-7 in AFRH)
    GPIOB->AFRH |= (4U << 0) | (4U << 4);

    // 3. I2C Configuration
    
    // Reset I2C: Set SWRST (Software Reset) bit in CR1
    I2C1->CR1 |= I2C_CR1_SWRST;
    // Clear SWRST bit to release reset and allow configuration
    I2C1->CR1 &= ~I2C_CR1_SWRST;

    // CR2: Set Peripheral Clock Frequency
    // APB1 Clock is 16MHz (default HSI). Set FREQ bits (0-5) to 16.
    I2C1->CR2 = 16U; 

    // CCR (Clock Control Register): Set I2C Speed
    // For Standard Mode 100kHz:
    // Thigh + Tlow = 10us (Period of 100kHz)
    // T_pclk = 62.5ns (1/16MHz)
    // CCR = F_pclk / (2 * F_i2c) = 16000000 / (2 * 100000) = 80.
    I2C1->CCR = 80U;

    // TRISE (Rise Time Register):
    // Max Rise Time for Standard Mode = 1000ns.
    // TRISE = (Max Rise Time / T_pclk) + 1
    // 1000ns / 62.5ns = 16.
    // TRISE = 16 + 1 = 17.
    I2C1->TRISE = 17U;

    // Enable I2C Peripheral by setting PE bit in CR1
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
    volatile uint32_t timeout; // Volatile to prevent loop optimization

    // Wait until I2C bus is not busy (BUSY flag in SR2)
    timeout = I2C_TIMEOUT;
    while((I2C1->SR2 & I2C_SR2_BUSY) && (timeout > 0U)){ timeout--; }
    if(timeout == 0U) return; // Error: Bus Busy stuck

    // Generate START condition (START bit in CR1)
    I2C1->CR1 |= I2C_CR1_START;
    
    // Wait for Start Bit (SB) generated flag in SR1
    timeout = I2C_TIMEOUT;
    while(!(I2C1->SR1 & I2C_SR1_SB) && (timeout > 0U)){ timeout--; }
    if(timeout == 0U) return; // Error: Start bit not set
    
    // Send 7-bit Address. Shift LSB is 0 for Write operation.
    // I2C Standard: Address is transmitted in bits 7:1
    I2C1->DR = addr; 
    
    // Wait for Address matched (ADDR) flag in SR1
    timeout = I2C_TIMEOUT;
    while(!(I2C1->SR1 & I2C_SR1_ADDR) && (timeout > 0U)){ timeout--; }
    if(timeout == 0U) return; // Error: Address not acknowledged
    
    // Clear ADDR flag: This is done by reading SR1 (done in loop) followed by reading SR2.
    (void)I2C1->SR2; 
    
    // Wait for Transmit Empty (TXE) flag in SR1
    timeout = I2C_TIMEOUT;
    while(!(I2C1->SR1 & I2C_SR1_TXE) && (timeout > 0U)){ timeout--; }
    
    // Send Register Address as the first data byte
    I2C1->DR = reg;
    
    // Loop to send remaining data bytes from buffer
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
    
    // Wait for Byte Transfer Finished (BTF) flag in SR1
    // This ensures last byte is physically transmitted and ACK'd before we send STOP.
    timeout = I2C_TIMEOUT;
    while(!(I2C1->SR1 & I2C_SR1_BTF) && (timeout > 0U)){ timeout--; }
    
    // Generate STOP condition (STOP bit in CR1)
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
    uint8_t tmp = data; // Store data in a temp variable
    I2C1_WriteMulti(addr, reg, &tmp, 1); // Reuse WriteMulti for single byte
}

// Stubs for Generic APIs if needed by other modules in future
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi) {}
void I2C_Init(I2C_Handle_t *pI2CHandle) {}
void I2C_DeInit(I2C_RegDef_t *pI2Cx) {}
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr) {}
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr) {}
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx , uint32_t FlagName) { return 0; }
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t AppEv) {}
