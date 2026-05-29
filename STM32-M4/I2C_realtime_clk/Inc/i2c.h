/*
 * i2c.h
 *
 *  Created on: 30-May-2026
 *      Author: athul
 */
#ifndef I2C_H_
#define I2C_H_

#include "stm32_f446xx.h"
#include <stdint.h>

/* Initialize I2C1 */
void I2C1_Init(void);

/* Read one byte from slave register */
void I2C1_Read(uint8_t slave_addr,
               uint8_t reg_addr,
               uint8_t *data);

/* Write one byte to slave register */
void I2C1_Write(uint8_t slave_addr,
                uint8_t reg_addr,
                uint8_t data);

#endif /* I2C_H_ */
