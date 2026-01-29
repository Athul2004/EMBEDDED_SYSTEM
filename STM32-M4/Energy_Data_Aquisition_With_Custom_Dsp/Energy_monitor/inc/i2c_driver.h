/*
 * i2c_driver.h
 * I2C Driver for STM32F446xx MCU
 */

#ifndef INC_I2C_DRIVER_H_
#define INC_I2C_DRIVER_H_

#include "stm32_f446xx.h"

// Compatibility definitions
typedef I2C_TypeDef I2C_RegDef_t;

/*
 * =========================================================================================
 *                                     I2C BIT DEFINITIONS
 * =========================================================================================
 */

// I2C Control Register 1 (CR1)
#define I2C_CR1_PE          (1U << 0)   // Peripheral Enable
#define I2C_CR1_START       (1U << 8)   // Start Generation
#define I2C_CR1_STOP        (1U << 9)   // Stop Generation
#define I2C_CR1_ACK         (1U << 10)  // Acknowledge Enable
#define I2C_CR1_SWRST       (1U << 15)  // Software Reset

// I2C Status Register 1 (SR1)
#define I2C_SR1_SB          (1U << 0)   // Start Bit (Master mode)
#define I2C_SR1_ADDR        (1U << 1)   // Address sent/matched
#define I2C_SR1_BTF         (1U << 2)   // Byte transfer finished
#define I2C_SR1_ADD10       (1U << 3)   // 10-bit header sent
#define I2C_SR1_STOPF       (1U << 4)   // Stop detection
#define I2C_SR1_RXNE        (1U << 6)   // Data register not empty
#define I2C_SR1_TXE         (1U << 7)   // Data register empty
#define I2C_SR1_BERR        (1U << 8)   // Bus error
#define I2C_SR1_ARLO        (1U << 9)   // Arbitration lost
#define I2C_SR1_AF          (1U << 10)  // Acknowledge failure
#define I2C_SR1_OVR         (1U << 11)  // Overrun/Underrun
#define I2C_SR1_PECERR      (1U << 12)  // PEC Error
#define I2C_SR1_TIMEOUT     (1U << 14)  // Timeout or Tlow error
#define I2C_SR1_SMBALERT    (1U << 15)  // SMBus alert

// I2C Status Register 2 (SR2)
#define I2C_SR2_BUSY        (1U << 1)   // Bus Busy

/*
 * Note: The following existing I2C_FLAG_ macros were in bitmask format (1 << pos)
 * internally in the old header but named like bit POSITIONS in some places.
 * We are standardizing on masks.
 */
#define I2C_FLAG_TXE   		I2C_SR1_TXE
#define I2C_FLAG_RXNE   	I2C_SR1_RXNE
#define I2C_FLAG_SB			I2C_SR1_SB
#define I2C_FLAG_BTF  		I2C_SR1_BTF
#define I2C_FLAG_ADDR 		I2C_SR1_ADDR
#define I2C_FLAG_STOPF 		I2C_SR1_STOPF

/*
 * Configuration structure for I2Cx peripheral
 */
typedef struct
{
	uint32_t I2C_SCLSpeed;
	uint8_t  I2C_DeviceAddress;
	uint8_t  I2C_AckControl;
	uint8_t  I2C_FMDutyCycle;
}I2C_Config_t;

/*
 * Handle structure for I2Cx peripheral
 */
typedef struct
{
	I2C_RegDef_t 	*pI2Cx;
	I2C_Config_t 	I2C_Config;
	uint8_t 		*pTxBuffer;
	uint8_t 		*pRxBuffer;
	uint32_t 		TxLen;
	uint32_t 		RxLen;
	uint8_t 		TxRxState;
	uint8_t 		DevAddr;
    uint32_t        RxSize;
    uint8_t         Sr;
}I2C_Handle_t;


/*
 * @I2C_SCLSpeed
 */
#define I2C_SCL_SPEED_SM 	100000U
#define I2C_SCL_SPEED_FM4K 	400000U
#define I2C_SCL_SPEED_FM2K  200000U

/*
 * @I2C_AckControl
 */
#define I2C_ACK_ENABLE        1U
#define I2C_ACK_DISABLE       0U

/*
 * @I2C_FMDutyCycle
 */
#define I2C_FM_DUTY_2        0U
#define I2C_FM_DUTY_16_9     1U

/*
 * Application states
 */
#define I2C_READY 					0U
#define I2C_BUSY_IN_RX 				1U
#define I2C_BUSY_IN_TX 				2U


/******************************************************************************************
 *								APIs supported by this driver
 ******************************************************************************************/

/*
 * Peripheral Clock setup
 */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void I2C1_Init(void); // Keeping specific init for this project
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

/*
 * Data Send and Receive
 */
void I2C1_WriteMulti(uint8_t addr, uint8_t reg, uint8_t* d, uint16_t c);
void I2C1_Write(uint8_t addr, uint8_t reg, uint8_t data);

// Generic APIs (Cleaned up prototypes)
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr);
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr);

uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx , uint32_t FlagName);
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t AppEv);

#endif /* INC_I2C_DRIVER_H_ */
