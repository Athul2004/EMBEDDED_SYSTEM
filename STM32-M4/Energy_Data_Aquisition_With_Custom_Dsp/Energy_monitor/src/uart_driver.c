/*
 * uart_driver.c
 * UART Driver Implementation for STM32F446xx
 * MISRA Refactored
 */

#include "uart_driver.h" // Include UART driver header

// System Clock Definition (Needed for Baud Rate)
#define PCLK1_FREQ 16000000U // 16 MHz Clock Source

/*********************************************************************
 * @brief             - Enables or disables peripheral clock for the given USART peripheral
 * @param[in]         - pUSARTx: Pointer to USART peripheral base address
 * @param[in]         - EnOrDi: Enable or Disable
 * @return            - None
 */
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		if(pUSARTx == USART1)
		{
             RCC->APB2ENR |= (1U << 4); // Enable USART1 Clock
		}else if(pUSARTx == USART2)
		{
             RCC->APB1ENR |= (1U << 17); // Enable USART2 Clock
		}else if(pUSARTx == USART3)
		{
             RCC->APB1ENR |= (1U << 18); // Enable USART3 Clock
		}
        else if(pUSARTx == UART4)
		{
             RCC->APB1ENR |= (1U << 19); // Enable UART4 Clock
		}
        else if(pUSARTx == UART5)
		{
             RCC->APB1ENR |= (1U << 20); // Enable UART5 Clock
		}
        else if(pUSARTx == USART6)
		{
             RCC->APB2ENR |= (1U << 5); // Enable USART6 Clock
		}
	}else
	{
		// Disable logic to be added if needed
	}
}

/*********************************************************************
 * @brief             - Sets the baud rate in BRR register
 * @param[in]         - pUSARTx: Pointer to USART peripheral
 * @param[in]         - BaudRate: Desired Baud Rate
 */
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate)
{
	uint32_t usartdiv; // Calculated USART Divider
	uint32_t M_part, F_part; // Mantissa and Fraction parts

    // Formula: USARTDIV = fCK / (8 * (2 - OVER8) * BaudRate)
    // Assuming OVER8 = 0 (Oversampling by 16) -> fCK / (16 * BaudRate)
    
	if(pUSARTx->CR1 & USART_CR1_OVER8)
	{
		//OVER8 = 1 , over sampling by 8
		usartdiv = ((25U * PCLK1_FREQ) / (2U * BaudRate));
	}else
	{
		//over sampling by 16 (Standard)
		usartdiv = ((25U * PCLK1_FREQ) / (4U * BaudRate));
	}

    // Calculate Mantissa: Integer part of division by 100 
    // (Note: The formula above multiplied by 25/100 to avoid floats)
	M_part = usartdiv / 100U;
    
    // Shift Mantissa to bits 15:4
	pUSARTx->BRR = M_part << 4;

    // Calculate Fraction: Remainder of division
	F_part = (usartdiv - (M_part * 100U));

	if(pUSARTx->CR1 & USART_CR1_OVER8)
	{
		//OVER8 = 1
        // Scale fractional part to 3 bits
		F_part = (((F_part * 8U) + 50U) / 100U) & ((uint8_t)0x07);

	}else
	{
		//OVER8 = 0
        // Scale fractional part to 4 bits
		F_part = (((F_part * 16U) + 50U) / 100U) & ((uint8_t)0x0F);
	}

    // Add Fraction to BRR
	pUSARTx->BRR |= F_part;
}

/*********************************************************************
 * @brief             - Initializes the USART peripheral
 * @param[in]         - pUSARTHandle: Pointer to handle structure
 */
void USART_Init(USART_Handle_t *pUSARTHandle)
{
	uint32_t tempreg=0;

	/******************************** Configuration of CR1******************************************/

	// Enable Clock for this peripheral
	USART_PeriClockControl(pUSARTHandle->pUSARTx, ENABLE);

	// Configure Transfer Mode (Tx/Rx)
	// Read user config and set RE (Receiver Enable) or TE (Transmitter Enable) bits
	if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX)
	{
		tempreg |= USART_CR1_RE;
	}else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX)
	{
		tempreg |= USART_CR1_TE;
	}else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX)
	{
		tempreg |= (USART_CR1_RE | USART_CR1_TE);
	}

    // Configure Word Length (8 or 9 bits)
	tempreg |= pUSARTHandle->USART_Config.USART_WordLength << 12; // Careful with generic shifting, but macro is 0 or 1.

    // Configure Parity Control
	if ( pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN)
	{
		tempreg |= USART_CR1_PCE; // Enable Parity Control
        // EVEN is default if PS bit is 0

	}else if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD)
	{
	    tempreg |= USART_CR1_PCE; // Enable Parity Control
	    tempreg |= USART_CR1_PS;  // Set Parity Selection to Odd
	}

   // Program the calculated value to CR1 register
	pUSARTHandle->pUSARTx->CR1 = tempreg;

	/******************************** Configuration of CR2******************************************/

	tempreg=0;

	// Configure Stop Bits (1, 0.5, 2, 1.5)
	tempreg |= pUSARTHandle->USART_Config.USART_NoOfStopBits << 12; // STOP bits position

	// Program CR2
	pUSARTHandle->pUSARTx->CR2 = tempreg;

	/******************************** Configuration of CR3******************************************/

	tempreg=0;

	// Configure Hardware Flow Control (CTS/RTS)
	if ( pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS)
	{
		tempreg |= USART_CR3_CTSE;

	}else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS)
	{
		tempreg |= USART_CR3_RTSE;

	}else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS)
	{
		tempreg |= (USART_CR3_CTSE | USART_CR3_RTSE);
	}

    // Program CR3
	pUSARTHandle->pUSARTx->CR3 = tempreg;

	/******************************** Configuration of BRR ******************************************/

    // Calculate and Set Baud Rate
    USART_SetBaudRate(pUSARTHandle->pUSARTx, pUSARTHandle->USART_Config.USART_Baud);

    // Enable USART Peripheral (UE Bit)
    pUSARTHandle->pUSARTx->CR1 |= USART_CR1_UE;
}

/*********************************************************************
 * @brief             - Sends data byte by byte (Blocking)
 * @param[in]         - pUSARTHandle: Handle structure
 * @param[in]         - pTxBuffer: Pointer to data buffer
 * @param[in]         - Len: Length of data
 */
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{
	uint16_t *pdata;
	// Loop over all bytes to send
	for(uint32_t i = 0 ; i < Len; i++)
	{
		// Wait until TXE (Transmit Empty) flag is set
        // This indicates DR register is ready for new data
		while(! (pUSARTHandle->pUSARTx->SR & USART_FLAG_TXE));

        // Start transmission
        // Check for 9-bit mode
		if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
		{
            // If 9-bit, load 2 bytes from buffer as 16-bit
			pdata = (uint16_t*) pTxBuffer;
            // Mask to 9 bits
			pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF);
			
            // If parity disabled, we consume 2 bytes of buffer for 9 bits of payload
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				pTxBuffer++;
				pTxBuffer++;
			}
			else
			{
                // Parity enabled, 9th bit is parity, so consume 1 byte
				pTxBuffer++;
			}
		}
		else
		{
            // 8-bit mode
			pUSARTHandle->pUSARTx->DR = (*pTxBuffer & (uint8_t)0xFF);
			pTxBuffer++; // Increment buffer pointer
		}
	}

	// Wait till TC (Transmission Complete) flag is set
    // This ensures the last frame is physically out of the shift register
	while( ! (pUSARTHandle->pUSARTx->SR & USART_FLAG_TC));
}

/*********************************************************************
 * @brief             - Receives data byte by byte (Blocking)
 * @param[in]         - pUSARTHandle: Handle structure
 * @param[in]         - pRxBuffer: Pointer to data buffer
 * @param[in]         - Len: Length of data
 */
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	// Loop to receive bytes
	for(uint32_t i = 0 ; i < Len; i++)
	{
		// Wait until RXNE (Read Not Empty) flag is set
        // This indicates data is ready to be read from DR
		while(! (pUSARTHandle->pUSARTx->SR & USART_FLAG_RXNE));

        // Check for 9-bit mode
		if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
		{
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
                // Read 9 bits
				*((uint16_t*) pRxBuffer) = (pUSARTHandle->pUSARTx->DR  & (uint16_t)0x01FF);
				pRxBuffer++;
				pRxBuffer++;
			}
			else
			{
                 // Read 8 bits (Parity excluded)
				 *pRxBuffer = (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);
				pRxBuffer++;
			}
		}
		else
		{
            // 8-bit mode
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
                // Read 8 bits
				 *pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);
			}
			else
			{
                // Read 7 bits (Parity bit is the 8th)
				 *pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->DR  & (uint8_t)0x7F);
			}
			pRxBuffer++;
		}
	}
}

// Stubs for Interrupt Driven Features (Placeholder)
// These functions are defined but not implemented for this blocking-only driver
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len) { return 0; }
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pRxBuffer, uint32_t Len) { return 0; }
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi) {}
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority) {}
void USART_IRQHandling(USART_Handle_t *pUSARTHandle) {}
void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t ApEv) {}
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi) {}
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint8_t StatusFlagName) { return 0; }
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName) {}


// ==========================================
// LEGACY SUPPORT IMPLEMENTATION
// ==========================================

// Legacy Function: Initialize UART2 with fixed settings (115200 8N1)
void UART2_Init(void) {
    ENABLE_GPIOA(); // Enable GPIOA Clock
    
    // GPIO Config: PA2(TX), PA3(RX) -> AF7 (USART2)
    // MODER: 10 (AF) for pin 2, 3 -> (2U << 4) | (2U << 6)
    GPIOA->MODER |=  (2U << 4) | (2U << 6); 
    // AFRL: Set AF7 (0111) for Pins 2 and 3
    GPIOA->AFRL &= ~((0xFU << 8) | (0xFU << 12)); 
    GPIOA->AFRL |=  (7U << 8) | (7U << 12);

    // Create a temporary handle to configure USART2
    USART_Handle_t usart2_handle;
    
    usart2_handle.pUSARTx = USART2;
    usart2_handle.USART_Config.USART_Mode = USART_MODE_TXRX;
    usart2_handle.USART_Config.USART_Baud = USART_STD_BAUD_115200;
    usart2_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
    usart2_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
    usart2_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
    usart2_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
    
    // Call generic init
    USART_Init(&usart2_handle);
}

// Legacy Function: Send a string via UART2
void UART2_SendString(char *string) {
    while(*string) {
        USART_Handle_t handle;
        handle.pUSARTx = USART2;
        handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS; 
        handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
        // Send single byte created from the string pointer
        USART_SendData(&handle, (uint8_t*)string, 1);
        string++;
    }
}

// Legacy Function: Send a number via UART2 (Integer to ASCII)
void UART2_SendNumber(int number) {
    char buf[12]; // Buffer for conversion
    int i = 0;
    
    // Handle specific cases
    if(number == 0) { UART2_SendString("0"); return; }
    if(number < 0) { UART2_SendString("-"); number = -number; }
    
    // Convert to reverse string
    while(number > 0) { buf[i++] = (number % 10) + '0'; number /= 10; }
    
    USART_Handle_t handle;
    handle.pUSARTx = USART2;
    handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS; 
    handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
    
    // Send in correct order
    while(--i >= 0) {
        USART_SendData(&handle, (uint8_t*)&buf[i], 1);
    }
}

// Legacy Function: Read a single char from UART2
char UART2_GetChar(void) {
    uint8_t c;
    USART_Handle_t handle;
    handle.pUSARTx = USART2;
    handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS; 
    handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
    
    USART_ReceiveData(&handle, &c, 1);
    return (char)c;
}

// Legacy Function: Send a single char via UART2
void UART2_SendChar(char c) {
    USART_Handle_t handle;
    handle.pUSARTx = USART2;
    handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS; 
    handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
    
    USART_SendData(&handle, (uint8_t*)&c, 1);
}
