/*
 * uart.h
 *
 *  Created on: 30-May-2026
 *      Author: athul
 */

#ifndef UART_H_
#define UART_H_

#include "stm32_f446xx.h"

void UART2_Init(void);
void UART2_WriteChar(char c);
void UART2_WriteString(char *str);

#endif
