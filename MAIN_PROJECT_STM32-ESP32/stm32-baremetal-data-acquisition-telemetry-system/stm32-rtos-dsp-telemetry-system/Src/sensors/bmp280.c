#include "sensors/bmp280.h"
#include "drivers/stm32f446xx_i2c_driver.h"
#include "drivers/delay.h"
#include <math.h>

extern I2C_Handle_t I2C1Handle;

static BMP280_CalibData calib_data;
static int32_t t_fine;

static void read_calibration_data(void) {
    uint8_t data[24];
    uint8_t reg = BMP280_REG_CALIB00;
    I2C_MasterSendData(&I2C1Handle, &reg, 1, BMP280_I2C_ADDR, I2C_ENABLE_SR);
    I2C_MasterReceiveData(&I2C1Handle, data, 24, BMP280_I2C_ADDR, I2C_DISABLE_SR);

    calib_data.dig_T1 = (data[1] << 8) | data[0];
    calib_data.dig_T2 = (data[3] << 8) | data[2];
    calib_data.dig_T3 = (data[5] << 8) | data[4];

    calib_data.dig_P1 = (data[7] << 8) | data[6];
    calib_data.dig_P2 = (data[9] << 8) | data[8];
    calib_data.dig_P3 = (data[11] << 8) | data[10];
    calib_data.dig_P4 = (data[13] << 8) | data[12];
    calib_data.dig_P5 = (data[15] << 8) | data[14];
    calib_data.dig_P6 = (data[17] << 8) | data[16];
    calib_data.dig_P7 = (data[19] << 8) | data[18];
    calib_data.dig_P8 = (data[21] << 8) | data[20];
    calib_data.dig_P9 = (data[23] << 8) | data[22];
}

uint8_t BMP280_Init(void) {
    uint8_t id = 0;
    uint8_t reg = BMP280_REG_ID;

    // Check device ID
    I2C_MasterSendData(&I2C1Handle, &reg, 1, BMP280_I2C_ADDR, I2C_ENABLE_SR);
    I2C_MasterReceiveData(&I2C1Handle, &id, 1, BMP280_I2C_ADDR, I2C_DISABLE_SR);
    if (id != 0x58) {
        return 0; // Device not found
    }

    // Reset the device
    uint8_t buf[2];
    buf[0] = BMP280_REG_RESET;
    buf[1] = 0xB6;
    I2C_MasterSendData(&I2C1Handle, buf, 2, BMP280_I2C_ADDR, I2C_DISABLE_SR);
    delay_ms(10); // Wait for reset

    // Read calibration data
    read_calibration_data();

    // Set config: standby 1ms, filter off, SPI off
    buf[0] = BMP280_REG_CONFIG;
    buf[1] = (0 << 5) | (0 << 2) | 0;
    I2C_MasterSendData(&I2C1Handle, buf, 2, BMP280_I2C_ADDR, I2C_DISABLE_SR);

    // Set ctrl_meas: temp oversampling x1, pressure oversampling x1, normal mode
    buf[0] = BMP280_REG_CTRL_MEAS;
    buf[1] = (1 << 5) | (1 << 2) | 3;
    I2C_MasterSendData(&I2C1Handle, buf, 2, BMP280_I2C_ADDR, I2C_DISABLE_SR);

    return 1; // Initialization successful
}

// Compensate temperature and return in Celsius
static float compensate_temperature(int32_t adc_T) {
    int32_t var1, var2, T;
    
    var1 = ((((adc_T >> 3) - ((int32_t)calib_data.dig_T1 << 1))) * ((int32_t)calib_data.dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)calib_data.dig_T1)) * ((adc_T >> 4) - ((int32_t)calib_data.dig_T1))) >> 12) * ((int32_t)calib_data.dig_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    
    return (float)T / 100.0f;
}

// Compensate pressure and return in Pa
static float compensate_pressure(int32_t adc_P) {
    int64_t var1, var2, p;
    
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)calib_data.dig_P6;
    var2 = var2 + ((var1 * (int64_t)calib_data.dig_P5) << 17);
    var2 = var2 + (((int64_t)calib_data.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)calib_data.dig_P3) >> 8) + ((var1 * (int64_t)calib_data.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)calib_data.dig_P1) >> 33;
    
    if (var1 == 0) {
        return 0; // Avoid division by zero
    }
    
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)calib_data.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)calib_data.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)calib_data.dig_P7) << 4);
    
    return (float)p / 256.0f;
}

void BMP280_ReadData(float* temperature, float* pressure, float* altitude) {
    uint8_t data[6];
    uint8_t reg = BMP280_REG_PRESS_MSB;
    
    // Read raw pressure and temperature
    I2C_MasterSendData(&I2C1Handle, &reg, 1, BMP280_I2C_ADDR, I2C_ENABLE_SR);
    I2C_MasterReceiveData(&I2C1Handle, data, 6, BMP280_I2C_ADDR, I2C_DISABLE_SR);
    
    int32_t adc_P = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
    int32_t adc_T = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4);
    
    *temperature = compensate_temperature(adc_T);
    *pressure = compensate_pressure(adc_P);
    
    // Calculate altitude using the hypsometric formula
    // P0 is standard sea level pressure (101325 Pa)
    if (altitude != NULL) {
        *altitude = 44330.0f * (1.0f - pow(*pressure / 101325.0f, 0.1903f));
    }
}
