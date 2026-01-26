/*
 * uart_driver.c
 * UART Driver Implementation for STM32F446xx
 */

#include "uart_driver.h"        // Driver Header
#include "stm32_f446xx.h"       // Device Header

/*
 * @brief  Initializes UART2 (115200 Baud, 8N1)
 * @param  None
 * @retval None
 */
// --- UART ---
void UART2_Init(void) {
    // 1. Enable Clocks
    ENABLE_GPIOA();             // Enable Clock for GPIO Port A (TX/RX Pins)
    ENABLE_UART2();             // Enable Clock for USART2 Peripheral

    // 2. Configure GPIO Pins (PA2 = TX, PA3 = RX)
    
    // MODER: Set PA2, PA3 to Alternate Function (10)
    // Bits 4-5 (PA2), 6-7 (PA3). 2U = 10.
    GPIOA->MODER |=  (2U << 4) | (2U << 6);
    
    // AFRL: Set Alternate Function 7 (AF7) for USART2
    // Bits 8-11 (PA2), 12-15 (PA3). 7U = 0111.
    // Clear bits first (safety) then set.
    GPIOA->AFRL &= ~((0xFU << 8) | (0xFU << 12)); // Clear AF
    GPIOA->AFRL |=  (7U << 8) | (7U << 12);       // Set AF7

    // 3. Configure USART2 Peripheral
    
    // BRR (Baud Rate Register): Set Baud Rate to 115200
    // PCLK1 = 16MHz. 16000000 / (16 * 115200) = 8.68.
    // Fraction = 0.68 * 16 = 10.88 ~ 11 (0xB). Mantissa = 8.
    // BRR = 0x8B.
    USART2->BRR = 0x008B;
    
    // CR1 (Control Register 1): Enable USART, TX, RX
    // Bit 13 (UE) = 1: USART Enable
    // Bit 3  (TE) = 1: Transmitter Enable
    // Bit 2  (RE) = 1: Receiver Enable
    USART2->CR1 = (1U << 13) | (1U << 3) | (1U << 2);
}

/*
 * @brief  Transmit a Null-Terminated String
 * @param  string: Pointer to string character array
 * @retval None
 */
void UART2_SendString(char *string) {
    // Loop until null terminator '\0' is reached
    while(*string) {
        // Wait until Transmit Data Register Empty (TXE) flag is set (SR Bit 7)
        while (!(USART2->SR & (1U << 7)));
        
        // Write character to Data Register and increment pointer
        USART2->DR = *string++;
    }
}

/*
 * @brief  Transmit a Signed Integer
 * @param  number: Integer value to transmit
 * @retval None
 */
void UART2_SendNumber(int number) {
    char buf[12];   // Buffer to hold ASCII representation (max size for 32-bit int)
    int i = 0;      // Index for buffer
    
    // Handle special case: 0
    if(number == 0) { 
        UART2_SendString("0"); 
        return; 
    }
    
    // Handle negative numbers
    if(number < 0) { 
        UART2_SendString("-");  // Send minus sign
        number = -number;       // Make number positive
    }
    
    // Convert number to string in reverse order (least significant digit first)
    while(number > 0) { 
        buf[i++] = (number % 10) + '0'; // Extract last digit, convert to ASCII
        number /= 10;                   // Move to next digit
    }
    
    // Transmit buffer in correct order (reverse of storage)
    while(--i >= 0) { 
        // Wait for TXE
        while(!(USART2->SR & (1U << 7))); 
        // Send character
        USART2->DR = buf[i]; 
    }
}

/*
 * @brief  Receive a single character
 * @param  None
 * @retval received char
 */
char UART2_GetChar(void) {
    // Wait until Read Data Register Not Empty (RXNE) flag is set (SR Bit 5)
    while (!(USART2->SR & (1U << 5)));
    
    // Read and return data from Data Register
    return (char)USART2->DR;
}
