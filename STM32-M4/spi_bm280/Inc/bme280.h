/*
 * bme280.h
 *
 *  Created on: 29-May-2026
 *      Author: athul
 */

#ifndef BME280_H_
#define BME280_H_

#include "stm32_f446xx.h"
#include "spi.h"

/* ==========================================
                BMP280 REGISTERS
========================================== */

#define BMP280_CHIP_ID_REG      0xD0
#define BMP280_RESET_REG        0xE0
#define BMP280_STATUS_REG       0xF3
#define BMP280_CTRL_MEAS_REG    0xF4
#define BMP280_CONFIG_REG       0xF5

/* Pressure Registers */

#define BMP280_PRESS_MSB        0xF7
#define BMP280_PRESS_LSB        0xF8
#define BMP280_PRESS_XLSB       0xF9

/* Temperature Registers */

#define BMP280_TEMP_MSB         0xFA
#define BMP280_TEMP_LSB         0xFB
#define BMP280_TEMP_XLSB        0xFC

/* Calibration Registers */

#define BMP280_CALIB_START      0x88

/* ==========================================
                DATA STRUCTURE
========================================== */

typedef struct
{
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;

    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;

}BMP280_CalibData_t;

/* ==========================================
                API FUNCTIONS
========================================== */

/* Sensor Init */

void BMP280_Init(void);

/* Basic Register Access */

uint8_t BMP280_ReadReg(uint8_t reg);

void BMP280_WriteReg(uint8_t reg, uint8_t value);

/* Device Check */

uint8_t BMP280_ReadChipID(void);

/* Calibration */

void BMP280_ReadCalibrationData(void);

/* Raw Sensor Data */

int32_t BMP280_ReadRawTemperature(void);

int32_t BMP280_ReadRawPressure(void);

/* Compensated Data */

int32_t BMP280_ReadTemperature(void);

uint32_t BMP280_ReadPressure(void);

/* Global Calibration Data */

extern BMP280_CalibData_t bmp280_calib;

/* Temperature Fine Resolution Variable */

extern int32_t t_fine;

#endif
