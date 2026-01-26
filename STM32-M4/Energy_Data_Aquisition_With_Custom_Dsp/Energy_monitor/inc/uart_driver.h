#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#include "stm32_f446xx.h"

void UART2_Init(void);
void UART2_SendChar(char c);
void UART2_SendString(char *string);
void UART2_SendNumber(int number);
char UART2_GetChar(void);

#endif
