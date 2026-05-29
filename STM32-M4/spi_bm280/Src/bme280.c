/*
 * bme280.c
 *
 *  Created on: 29-May-2026
 *      Author: athul
 */


#include "bme280.h"

/* =====================================================
                    GLOBAL VARIABLES
===================================================== */

BMP280_CalibData_t bmp280_calib;

int32_t t_fine;

/* =====================================================
                    CS CONTROL
===================================================== */

static void BMP280_CS_LOW(void)
{
    GPIOA->ODR &= ~(1 << 4);
}

static void BMP280_CS_HIGH(void)
{
    GPIOA->ODR |= (1 << 4);
}

/* =====================================================
                    SPI REGISTER ACCESS
===================================================== */

uint8_t BMP280_ReadReg(uint8_t reg)
{
    uint8_t data;

    reg |= 0x80;      // Read bit

    BMP280_CS_LOW();

    SPI1_Transfer(reg);

    data = SPI1_Transfer(0xFF);

    BMP280_CS_HIGH();

    return data;
}

void BMP280_WriteReg(uint8_t reg, uint8_t value)
{
    reg &= ~(0x80);   // Write bit

    BMP280_CS_LOW();

    SPI1_Transfer(reg);

    SPI1_Transfer(value);

    BMP280_CS_HIGH();
}

/* =====================================================
                    CHIP ID
===================================================== */

uint8_t BMP280_ReadChipID(void)
{
    return BMP280_ReadReg(BMP280_CHIP_ID_REG);
}

/* =====================================================
                    READ 16 BIT
===================================================== */

static uint16_t BMP280_Read16_LE(uint8_t reg)
{
    uint8_t lsb;
    uint8_t msb;

    lsb = BMP280_ReadReg(reg);
    msb = BMP280_ReadReg(reg + 1);

    return ((uint16_t)msb << 8) | lsb;
}

/* =====================================================
                CALIBRATION DATA
===================================================== */

void BMP280_ReadCalibrationData(void)
{
    bmp280_calib.dig_T1 = BMP280_Read16_LE(0x88);
    bmp280_calib.dig_T2 = (int16_t)BMP280_Read16_LE(0x8A);
    bmp280_calib.dig_T3 = (int16_t)BMP280_Read16_LE(0x8C);

    bmp280_calib.dig_P1 = BMP280_Read16_LE(0x8E);
    bmp280_calib.dig_P2 = (int16_t)BMP280_Read16_LE(0x90);
    bmp280_calib.dig_P3 = (int16_t)BMP280_Read16_LE(0x92);
    bmp280_calib.dig_P4 = (int16_t)BMP280_Read16_LE(0x94);
    bmp280_calib.dig_P5 = (int16_t)BMP280_Read16_LE(0x96);
    bmp280_calib.dig_P6 = (int16_t)BMP280_Read16_LE(0x98);
    bmp280_calib.dig_P7 = (int16_t)BMP280_Read16_LE(0x9A);
    bmp280_calib.dig_P8 = (int16_t)BMP280_Read16_LE(0x9C);
    bmp280_calib.dig_P9 = (int16_t)BMP280_Read16_LE(0x9E);
}

/* =====================================================
                    INITIALIZATION
===================================================== */

void BMP280_Init(void)
{
    /* Temperature x1
       Pressure x1
       Normal Mode */

    BMP280_WriteReg(BMP280_CTRL_MEAS_REG, 0x27);

    /* Standby 1000ms
       Filter OFF */

    BMP280_WriteReg(BMP280_CONFIG_REG, 0xA0);

    BMP280_ReadCalibrationData();
}

/* =====================================================
                RAW TEMPERATURE
===================================================== */

int32_t BMP280_ReadRawTemperature(void)
{
    int32_t adc_T;

    adc_T =
        ((uint32_t)BMP280_ReadReg(BMP280_TEMP_MSB) << 12) |
        ((uint32_t)BMP280_ReadReg(BMP280_TEMP_LSB) << 4 ) |
        ((uint32_t)BMP280_ReadReg(BMP280_TEMP_XLSB) >> 4 );

    return adc_T;
}

/* =====================================================
                RAW PRESSURE
===================================================== */

int32_t BMP280_ReadRawPressure(void)
{
    int32_t adc_P;

    adc_P =
        ((uint32_t)BMP280_ReadReg(BMP280_PRESS_MSB) << 12) |
        ((uint32_t)BMP280_ReadReg(BMP280_PRESS_LSB) << 4 ) |
        ((uint32_t)BMP280_ReadReg(BMP280_PRESS_XLSB) >> 4 );

    return adc_P;
}

/* =====================================================
            COMPENSATED TEMPERATURE
===================================================== */

int32_t BMP280_ReadTemperature(void)
{
    int32_t adc_T;

    int32_t var1;
    int32_t var2;
    int32_t T;

    adc_T = BMP280_ReadRawTemperature();

    var1 =
        ((((adc_T >> 3) -
        ((int32_t)bmp280_calib.dig_T1 << 1)))
        * ((int32_t)bmp280_calib.dig_T2)) >> 11;

    var2 =
        (((((adc_T >> 4) -
        ((int32_t)bmp280_calib.dig_T1))
        * ((adc_T >> 4) -
        ((int32_t)bmp280_calib.dig_T1))) >> 12)
        * ((int32_t)bmp280_calib.dig_T3)) >> 14;

    t_fine = var1 + var2;

    T = (t_fine * 5 + 128) >> 8;

    return T;     /* °C x100 */
}

/* =====================================================
                COMPENSATED PRESSURE
===================================================== */

uint32_t BMP280_ReadPressure(void)
{
    int64_t var1,var2,p;

    int32_t adc_P;

    adc_P = BMP280_ReadRawPressure();

    var1 = ((int64_t)t_fine) - 128000;

    var2 = var1 * var1 * (int64_t)bmp280_calib.dig_P6;

    var2 = var2 +
           ((var1 * (int64_t)bmp280_calib.dig_P5) << 17);

    var2 = var2 +
           (((int64_t)bmp280_calib.dig_P4) << 35);

    var1 =
        ((var1 * var1 *
        (int64_t)bmp280_calib.dig_P3) >> 8) +
        ((var1 *
        (int64_t)bmp280_calib.dig_P2) << 12);

    var1 =
        (((((int64_t)1) << 47) + var1))
        * ((int64_t)bmp280_calib.dig_P1) >> 33;

    if(var1 == 0)
        return 0;

    p = 1048576 - adc_P;

    p = (((p << 31) - var2) * 3125) / var1;

    var1 =
        (((int64_t)bmp280_calib.dig_P9)
        * (p >> 13)
        * (p >> 13)) >> 25;

    var2 =
        (((int64_t)bmp280_calib.dig_P8)
        * p) >> 19;

    p = ((p + var1 + var2) >> 8)
        + (((int64_t)bmp280_calib.dig_P7) << 4);

    return (uint32_t)(p / 256);
}
