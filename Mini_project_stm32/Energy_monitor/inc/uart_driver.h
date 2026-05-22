/*
 * uart_driver.h
 * USART/UART Driver for STM32F446xx MCU
 * MISRA Refactored
 */

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#include "stm32_f446xx.h"    // Include hardware definitions

// Compatibility Definition
typedef USART_TypeDef USART_RegDef_t;   // Alias definition for generic usage

/*
 * =========================================================================================
 *                                     USART BIT DEFINITIONS
 * =========================================================================================
 */

// USART Control Register 1 (CR1)
#define USART_CR1_RE            (1U << 2)   // Receiver Enable (Bit 2)
#define USART_CR1_TE            (1U << 3)   // Transmitter Enable (Bit 3)
#define USART_CR1_PS            (1U << 9)   // Parity Selection (Bit 9)
#define USART_CR1_PCE           (1U << 10)  // Parity Control Enable (Bit 10)
#define USART_CR1_M             (1U << 12)  // Word Length (Bit 12)
#define USART_CR1_UE            (1U << 13)  // USART Enable (Bit 13)
#define USART_CR1_OVER8         (1U << 15)  // Oversampling Mode (Bit 15)

// USART Control Register 2 (CR2)
#define USART_CR2_STOP_1        (0x0U << 12) // 1 Stop bit (Bits 12-13 -> 00)
#define USART_CR2_STOP_0_5      (0x1U << 12) // 0.5 Stop bit (Bits 12-13 -> 01)
#define USART_CR2_STOP_2        (0x2U << 12) // 2 Stop bits (Bits 12-13 -> 10)
#define USART_CR2_STOP_1_5      (0x3U << 12) // 1.5 Stop bits (Bits 12-13 -> 11)

// USART Control Register 3 (CR3)
#define USART_CR3_RTSE          (1U << 8)   // RTS Enable (Bit 8)
#define USART_CR3_CTSE          (1U << 9)   // CTS Enable (Bit 9)

// USART Status Register (SR)
#define USART_SR_RXNE           (1U << 5)   // Read Data Register Not Empty (Bit 5)
#define USART_SR_TC             (1U << 6)   // Transmission Complete (Bit 6)
#define USART_SR_TXE            (1U << 7)   // Transmit Data Register Empty (Bit 7)

// Flag Macros
#define USART_FLAG_TXE 			USART_SR_TXE    // Transmit Data Register Empty Flag
#define USART_FLAG_RXNE 		USART_SR_RXNE   // Receive Data Register Not Empty Flag
#define USART_FLAG_TC 			USART_SR_TC     // Transmission Complete Flag

/*
 * Configuration structure for USARTx peripheral
 */
typedef struct
{
	uint8_t USART_Mode;             // Mode: Rx, Tx, or RxTx
	uint32_t USART_Baud;            // Communication Baud Rate
	uint8_t USART_NoOfStopBits;     // Number of Stop Bits
	uint8_t USART_WordLength;       // Word Length (8 or 9 bits)
	uint8_t USART_ParityControl;    // Parity: None, Odd, Even
	uint8_t USART_HWFlowControl;    // Hardware Flow Control: CTS/RTS
}USART_Config_t;


/*
 * Handle structure for USARTx peripheral
 */
typedef struct
{
	USART_RegDef_t *pUSARTx;        // Pointer to USART peripheral base address
	USART_Config_t   USART_Config;  // USART Configuration structure
	uint8_t *pTxBuffer;             // Pointer to Tx buffer
	uint8_t *pRxBuffer;             // Pointer to Rx buffer
	uint32_t TxLen;                 // Length of Tx data
	uint32_t RxLen;                 // Length of Rx data
	uint8_t TxBusyState;            // Tx Busy state flag
	uint8_t RxBusyState;            // Rx Busy state flag
}USART_Handle_t;


/*
 *@USART_Mode
 */
#define USART_MODE_ONLY_TX 	0U      // Transmitter only mode
#define USART_MODE_ONLY_RX 	1U      // Receiver only mode
#define USART_MODE_TXRX  	2U      // Transmitter and Receiver mode

/*
 *@USART_Baud
 */
#define USART_STD_BAUD_1200					1200U       // 1200 bps
#define USART_STD_BAUD_2400					2400U       // 2400 bps
#define USART_STD_BAUD_9600					9600U       // 9600 bps
#define USART_STD_BAUD_19200 				19200U      // 19200 bps
#define USART_STD_BAUD_38400 				38400U      // 38400 bps
#define USART_STD_BAUD_57600 				57600U      // 57600 bps
#define USART_STD_BAUD_115200 				115200U     // 115200 bps
#define USART_STD_BAUD_230400 				230400U     // 230400 bps
#define USART_STD_BAUD_460800 				460800U     // 460800 bps
#define USART_STD_BAUD_921600 				921600U     // 921600 bps
#define USART_STD_BAUD_2M 					2000000U    // 2 Mbps
#define USART_STD_BAUD_3M 					3000000U    // 3 Mbps


/*
 *@USART_ParityControl
 */
#define USART_PARITY_EN_ODD   2U    // Odd Parity Enable
#define USART_PARITY_EN_EVEN  1U    // Even Parity Enable
#define USART_PARITY_DISABLE   0U   // Parity Disable

/*
 *@USART_WordLength
 */
#define USART_WORDLEN_8BITS  0U     // 8-bit word length
#define USART_WORDLEN_9BITS  1U     // 9-bit word length

/*
 *@USART_NoOfStopBits
 */
#define USART_STOPBITS_1     0U     // 1 Stop Bit
#define USART_STOPBITS_0_5   1U     // 0.5 Stop Bit
#define USART_STOPBITS_2     2U     // 2 Stop Bits
#define USART_STOPBITS_1_5   3U     // 1.5 Stop Bits


/*
 *@USART_HWFlowControl
 */
#define USART_HW_FLOW_CTRL_NONE    	0U      // No Flow Control
#define USART_HW_FLOW_CTRL_CTS    	1U      // CTS only
#define USART_HW_FLOW_CTRL_RTS    	2U      // RTS only
#define USART_HW_FLOW_CTRL_CTS_RTS	3U      // CTS and RTS


/*
 * Application states
 */
#define USART_BUSY_IN_RX 1U         // Rx Busy state
#define USART_BUSY_IN_TX 2U         // Tx Busy state
#define USART_READY 0U              // Ready state


/******************************************************************************************
 *								APIs supported by this driver
 ******************************************************************************************/

// Peripheral Clock Control
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi);

// Init and De-Init
void USART_Init(USART_Handle_t *pUSARTHandle);
void USART_DeInit(USART_Handle_t *pUSARTHandle);

// Data Transmission and Reception (Blocking)
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len);
void  USART_ReceiveData(USART_Handle_t *pUSARTHandle,uint8_t *pRxBuffer, uint32_t Len);

// Interrupt Data Transmission and Reception
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len);
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pRxBuffer, uint32_t Len);

// Interrupt Configuration
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void USART_IRQHandling(USART_Handle_t *pUSARTHandle);

// Control and Status
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint8_t StatusFlagName);
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName);
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi);
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate);
void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t ApEv);

// --- Legacy Support (Application Specific Wrapper) ---
void UART2_Init(void);                  // Initialize UART2 specifically
void UART2_SendChar(char c);            // Send single character via UART2
void UART2_SendString(char *string);    // Send string via UART2
void UART2_SendNumber(int number);      // Send integer as text via UART2
char UART2_GetChar(void);               // Receive char via UART2

#endif /* UART_DRIVER_H_ */
