/*
 * spi.h
 *
 *  Created on: 29-May-2026
 *      Author: athul
 */

#ifndef SPI_H_
#define SPI_H_

#include "stm32_f446xx.h"

void SPI1_Init(void);

void SPI1_SendData(uint8_t *pTxBuffer, uint32_t Len);

void SPI1_ReceiveData(uint8_t *pRxBuffer, uint32_t Len);

uint8_t SPI1_Transfer(uint8_t data);

#endif
