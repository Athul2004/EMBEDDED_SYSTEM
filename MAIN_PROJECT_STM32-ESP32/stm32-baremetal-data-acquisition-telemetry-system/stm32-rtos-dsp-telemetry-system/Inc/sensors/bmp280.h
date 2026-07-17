#ifndef BMP280_H
#define BMP280_H

#include <stdint.h>

// BMP280 I2C Address
#define BMP280_I2C_ADDR 0x76

// Register Addresses
#define BMP280_REG_CALIB00 0x88
#define BMP280_REG_ID 0xD0
#define BMP280_REG_RESET 0xE0
#define BMP280_REG_STATUS 0xF3
#define BMP280_REG_CTRL_MEAS 0xF4
#define BMP280_REG_CONFIG 0xF5
#define BMP280_REG_PRESS_MSB 0xF7
#define BMP280_REG_TEMP_MSB 0xFA

typedef struct {
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
} BMP280_CalibData;

uint8_t BMP280_Init(void);
void BMP280_ReadData(float* temperature, float* pressure, float* altitude);

#endif // BMP280_H
