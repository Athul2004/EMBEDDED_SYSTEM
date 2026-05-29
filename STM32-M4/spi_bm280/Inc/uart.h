/*
 * uart.h
 *
 *  Created on: 29-May-2026
 *      Author: athul
 */

#ifndef UART_H_
#define UART_H_

#include "stm32_f446xx.h"

void UART2_Init(void);

void UART2_SendChar(char ch);

void UART2_SendString(char *str);

void UART2_SendNumber(uint32_t num);

#endif
