/*
 * uart_driver.h
 * USART/UART Driver for STM32F446xx MCU
 * MISRA Refactored
 */

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#include "stm32_f446xx.h"

// Compatibility Definition
typedef USART_TypeDef USART_RegDef_t;

/*
 * =========================================================================================
 *                                     USART BIT DEFINITIONS
 * =========================================================================================
 */

// USART Control Register 1 (CR1)
#define USART_CR1_RE            (1U << 2)   // Receiver Enable
#define USART_CR1_TE            (1U << 3)   // Transmitter Enable
#define USART_CR1_PS            (1U << 9)   // Parity Selection
#define USART_CR1_PCE           (1U << 10)  // Parity Control Enable
#define USART_CR1_M             (1U << 12)  // Word Length
#define USART_CR1_UE            (1U << 13)  // USART Enable
#define USART_CR1_OVER8         (1U << 15)  // Oversampling Mode

// USART Control Register 2 (CR2)
#define USART_CR2_STOP_1        (0x0U << 12) // 1 Stop bit
#define USART_CR2_STOP_0_5      (0x1U << 12) // 0.5 Stop bit
#define USART_CR2_STOP_2        (0x2U << 12) // 2 Stop bits
#define USART_CR2_STOP_1_5      (0x3U << 12) // 1.5 Stop bits

// USART Control Register 3 (CR3)
#define USART_CR3_RTSE          (1U << 8)   // RTS Enable
#define USART_CR3_CTSE          (1U << 9)   // CTS Enable

// USART Status Register (SR)
#define USART_SR_RXNE           (1U << 5)   // Read Data Register Not Empty
#define USART_SR_TC             (1U << 6)   // Transmission Complete
#define USART_SR_TXE            (1U << 7)   // Transmit Data Register Empty

// Flag Macros
#define USART_FLAG_TXE 			USART_SR_TXE
#define USART_FLAG_RXNE 		USART_SR_RXNE
#define USART_FLAG_TC 			USART_SR_TC

/*
 * Configuration structure for USARTx peripheral
 */
typedef struct
{
	uint8_t USART_Mode;
	uint32_t USART_Baud;
	uint8_t USART_NoOfStopBits;
	uint8_t USART_WordLength;
	uint8_t USART_ParityControl;
	uint8_t USART_HWFlowControl;
}USART_Config_t;


/*
 * Handle structure for USARTx peripheral
 */
typedef struct
{
	USART_RegDef_t *pUSARTx;
	USART_Config_t   USART_Config;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t TxLen;
	uint32_t RxLen;
	uint8_t TxBusyState;
	uint8_t RxBusyState;
}USART_Handle_t;


/*
 *@USART_Mode
 */
#define USART_MODE_ONLY_TX 	0U
#define USART_MODE_ONLY_RX 	1U
#define USART_MODE_TXRX  	2U

/*
 *@USART_Baud
 */
#define USART_STD_BAUD_1200					1200U
#define USART_STD_BAUD_2400					2400U
#define USART_STD_BAUD_9600					9600U
#define USART_STD_BAUD_19200 				19200U
#define USART_STD_BAUD_38400 				38400U
#define USART_STD_BAUD_57600 				57600U
#define USART_STD_BAUD_115200 				115200U
#define USART_STD_BAUD_230400 				230400U
#define USART_STD_BAUD_460800 				460800U
#define USART_STD_BAUD_921600 				921600U
#define USART_STD_BAUD_2M 					2000000U
#define USART_STD_BAUD_3M 					3000000U


/*
 *@USART_ParityControl
 */
#define USART_PARITY_EN_ODD   2U
#define USART_PARITY_EN_EVEN  1U
#define USART_PARITY_DISABLE   0U

/*
 *@USART_WordLength
 */
#define USART_WORDLEN_8BITS  0U
#define USART_WORDLEN_9BITS  1U

/*
 *@USART_NoOfStopBits
 */
#define USART_STOPBITS_1     0U
#define USART_STOPBITS_0_5   1U
#define USART_STOPBITS_2     2U
#define USART_STOPBITS_1_5   3U


/*
 *@USART_HWFlowControl
 */
#define USART_HW_FLOW_CTRL_NONE    	0U
#define USART_HW_FLOW_CTRL_CTS    	1U
#define USART_HW_FLOW_CTRL_RTS    	2U
#define USART_HW_FLOW_CTRL_CTS_RTS	3U


/*
 * Application states
 */
#define USART_BUSY_IN_RX 1U
#define USART_BUSY_IN_TX 2U
#define USART_READY 0U


/******************************************************************************************
 *								APIs supported by this driver
 ******************************************************************************************/

void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi);

void USART_Init(USART_Handle_t *pUSARTHandle);
void USART_DeInit(USART_Handle_t *pUSARTHandle);

void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len);
void  USART_ReceiveData(USART_Handle_t *pUSARTHandle,uint8_t *pRxBuffer, uint32_t Len);
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len);
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pRxBuffer, uint32_t Len);

void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void USART_IRQHandling(USART_Handle_t *pUSARTHandle);

uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint8_t StatusFlagName);
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName);
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi);
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate);
void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t ApEv);

// --- Legacy Support ---
void UART2_Init(void);
void UART2_SendChar(char c);
void UART2_SendString(char *string);
void UART2_SendNumber(int number);
char UART2_GetChar(void);

#endif /* UART_DRIVER_H_ */
