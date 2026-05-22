/*
 * i2c_driver.h
 * I2C Driver for STM32F446xx MCU
 */

#ifndef INC_I2C_DRIVER_H_
#define INC_I2C_DRIVER_H_

#include "stm32_f446xx.h"    // Include hardware definitions

// Compatibility definitions for TypeDef
typedef I2C_TypeDef I2C_RegDef_t;

/*
 * =========================================================================================
 *                                     I2C BIT DEFINITIONS
 * =========================================================================================
 */

// I2C Control Register 1 (CR1)
#define I2C_CR1_PE          (1U << 0)   // Peripheral Enable bit (Bit 0)
#define I2C_CR1_START       (1U << 8)   // Start Generation bit (Bit 8)
#define I2C_CR1_STOP        (1U << 9)   // Stop Generation bit (Bit 9)
#define I2C_CR1_ACK         (1U << 10)  // Acknowledge Enable bit (Bit 10)
#define I2C_CR1_SWRST       (1U << 15)  // Software Reset bit (Bit 15)

// I2C Status Register 1 (SR1)
#define I2C_SR1_SB          (1U << 0)   // Start Bit flag (Master mode) (Bit 0)
#define I2C_SR1_ADDR        (1U << 1)   // Address sent/matched flag (Bit 1)
#define I2C_SR1_BTF         (1U << 2)   // Byte transfer finished flag (Bit 2)
#define I2C_SR1_ADD10       (1U << 3)   // 10-bit header sent flag (Bit 3)
#define I2C_SR1_STOPF       (1U << 4)   // Stop detection flag (Bit 4)
#define I2C_SR1_RXNE        (1U << 6)   // Data register not empty flag (Bit 6)
#define I2C_SR1_TXE         (1U << 7)   // Data register empty flag (Bit 7)
#define I2C_SR1_BERR        (1U << 8)   // Bus error flag (Bit 8)
#define I2C_SR1_ARLO        (1U << 9)   // Arbitration lost flag (Bit 9)
#define I2C_SR1_AF          (1U << 10)  // Acknowledge failure flag (Bit 10)
#define I2C_SR1_OVR         (1U << 11)  // Overrun/Underrun flag (Bit 11)
#define I2C_SR1_PECERR      (1U << 12)  // PEC Error flag (Bit 12)
#define I2C_SR1_TIMEOUT     (1U << 14)  // Timeout or Tlow error flag (Bit 14)
#define I2C_SR1_SMBALERT    (1U << 15)  // SMBus alert flag (Bit 15)

// I2C Status Register 2 (SR2)
#define I2C_SR2_BUSY        (1U << 1)   // Bus Busy flag (Bit 1)

/*
 * Note: The following existing I2C_FLAG_ macros were in bitmask format (1 << pos)
 * internally in the old header but named like bit POSITIONS in some places.
 * We are standardizing on masks.
 */
#define I2C_FLAG_TXE   		I2C_SR1_TXE     // Alias for Tx buffer empty flag
#define I2C_FLAG_RXNE   	I2C_SR1_RXNE    // Alias for Rx buffer not empty flag
#define I2C_FLAG_SB			I2C_SR1_SB      // Alias for Start Bit flag
#define I2C_FLAG_BTF  		I2C_SR1_BTF     // Alias for Byte Transfer Finished flag
#define I2C_FLAG_ADDR 		I2C_SR1_ADDR    // Alias for Address matched flag
#define I2C_FLAG_STOPF 		I2C_SR1_STOPF   // Alias for Stop detected flag

/*
 * Configuration structure for I2Cx peripheral
 */
typedef struct
{
	uint32_t I2C_SCLSpeed;          // I2C Clock speed (e.g., 100000)
	uint8_t  I2C_DeviceAddress;     // I2C Device address (7-bit)
	uint8_t  I2C_AckControl;        // Enable/Disable Acknowledgement
	uint8_t  I2C_FMDutyCycle;       // Fast Mode Duty Cycle selection
}I2C_Config_t;

/*
 * Handle structure for I2Cx peripheral
 */
typedef struct
{
	I2C_RegDef_t 	*pI2Cx;         // Pointer to I2C peripheral base address
	I2C_Config_t 	I2C_Config;     // Configuration structure
	uint8_t 		*pTxBuffer;     // Pointer to Tx buffer
	uint8_t 		*pRxBuffer;     // Pointer to Rx buffer
	uint32_t 		TxLen;          // Length of Tx data
	uint32_t 		RxLen;          // Length of Rx data
	uint8_t 		TxRxState;      // Current state of communication
	uint8_t 		DevAddr;        // Device address
    uint32_t        RxSize;         // Size of data to receive
    uint8_t         Sr;             // Repeated Start configuration
}I2C_Handle_t;


/*
 * @I2C_SCLSpeed
 */
#define I2C_SCL_SPEED_SM 	100000U     // Standard Mode Speed (100kHz)
#define I2C_SCL_SPEED_FM4K 	400000U     // Fast Mode Speed (400kHz)
#define I2C_SCL_SPEED_FM2K  200000U     // Fast Mode Speed (200kHz)

/*
 * @I2C_AckControl
 */
#define I2C_ACK_ENABLE        1U        // Acknowledgement Enabled
#define I2C_ACK_DISABLE       0U        // Acknowledgement Disabled

/*
 * @I2C_FMDutyCycle
 */
#define I2C_FM_DUTY_2        0U         // Duty cycle 2
#define I2C_FM_DUTY_16_9     1U         // Duty cycle 16/9

/*
 * Application states
 */
#define I2C_READY 					0U  // I2C Peripheral Ready state
#define I2C_BUSY_IN_RX 				1U  // I2C Peripheral Busy in Reception
#define I2C_BUSY_IN_TX 				2U  // I2C Peripheral Busy in Transmission


/******************************************************************************************
 *								APIs supported by this driver
 ******************************************************************************************/

/*
 * Peripheral Clock setup
 */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi); // Enable/Disable I2C Clock

/*
 * Init and De-init
 */
void I2C1_Init(void); // Keeping specific init for this project
void I2C_Init(I2C_Handle_t *pI2CHandle); // Generic Init
void I2C_DeInit(I2C_RegDef_t *pI2Cx);    // De-Init

/*
 * Data Send and Receive
 */
void I2C1_WriteMulti(uint8_t addr, uint8_t reg, uint8_t* d, uint16_t c); // Write generic data
void I2C1_Write(uint8_t addr, uint8_t reg, uint8_t data);                // Write single byte

// Generic APIs (Cleaned up prototypes)
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr); // Send data as Master
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr); // Receive data as Master

uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx , uint32_t FlagName); // Check flag status
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t AppEv); // Callback for application events

#endif /* INC_I2C_DRIVER_H_ */
