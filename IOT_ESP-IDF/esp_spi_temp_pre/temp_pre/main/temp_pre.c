#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  18
#define PIN_NUM_CS   5

#define BMP280_CHIP_ID_REG 0xD0

static const char *TAG = "BMP280";

spi_device_handle_t bmp280;

// Calibration data structure
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
} bmp280_calib_data_t;

bmp280_calib_data_t calib_data;
int32_t t_fine;

/* SPI Init */
void bmp280_spi_init(void)
{
    spi_bus_config_t buscfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 1000000,
        .mode = 0,
        .spics_io_num = PIN_NUM_CS,
        .queue_size = 1,
    };

    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &bmp280));

    ESP_LOGI(TAG, "SPI Initialized");
}

/* Read Register */
uint8_t bmp280_read_register(uint8_t reg)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    uint8_t tx[2] = {reg | 0x80, 0x00};
    uint8_t rx[2] = {0};

    t.length = 16;
    t.tx_buffer = tx;
    t.rx_buffer = rx;

    ESP_ERROR_CHECK(spi_device_transmit(bmp280, &t));

    return rx[1];
}

/* Write Register */
void bmp280_write_register(uint8_t reg, uint8_t data)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    uint8_t tx[2] = {reg & 0x7F, data};

    t.length = 16;
    t.tx_buffer = tx;

    ESP_ERROR_CHECK(spi_device_transmit(bmp280, &t));
}

/* Read multiple registers */
void bmp280_read_registers(uint8_t reg, uint8_t *data, uint8_t len)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    
    uint8_t tx[32];
    uint8_t rx[32];
    
    tx[0] = reg | 0x80;
    for(int i = 1; i <= len; i++) {
        tx[i] = 0x00;
    }
    
    t.length = (len + 1) * 8;
    t.tx_buffer = tx;
    t.rx_buffer = rx;
    
    ESP_ERROR_CHECK(spi_device_transmit(bmp280, &t));
    
    for(int i = 0; i < len; i++) {
        data[i] = rx[i + 1];
    }
}

/* Read Calibration Data */
void bmp280_read_calibration_data(void)
{
    uint8_t data[24];
    
    bmp280_read_registers(0x88, data, 24);
    
    calib_data.dig_T1 = (uint16_t)((data[1] << 8) | data[0]);
    calib_data.dig_T2 = (int16_t)((data[3] << 8) | data[2]);
    calib_data.dig_T3 = (int16_t)((data[5] << 8) | data[4]);
    calib_data.dig_P1 = (uint16_t)((data[7] << 8) | data[6]);
    calib_data.dig_P2 = (int16_t)((data[9] << 8) | data[8]);
    calib_data.dig_P3 = (int16_t)((data[11] << 8) | data[10]);
    calib_data.dig_P4 = (int16_t)((data[13] << 8) | data[12]);
    calib_data.dig_P5 = (int16_t)((data[15] << 8) | data[14]);
    calib_data.dig_P6 = (int16_t)((data[17] << 8) | data[16]);
    calib_data.dig_P7 = (int16_t)((data[19] << 8) | data[18]);
    calib_data.dig_P8 = (int16_t)((data[21] << 8) | data[20]);
    calib_data.dig_P9 = (int16_t)((data[23] << 8) | data[22]);
    
    ESP_LOGI(TAG, "Calibration data loaded");
    ESP_LOGI(TAG, "T1=%d, T2=%d, T3=%d", calib_data.dig_T1, calib_data.dig_T2, calib_data.dig_T3);
    ESP_LOGI(TAG, "P1=%d, P2=%d, P3=%d", calib_data.dig_P1, calib_data.dig_P2, calib_data.dig_P3);
}

/* Read Raw Temperature */
int32_t bmp280_read_raw_temp(void)
{
    uint8_t msb  = bmp280_read_register(0xFA);
    uint8_t lsb  = bmp280_read_register(0xFB);
    uint8_t xlsb = bmp280_read_register(0xFC);

    return ((int32_t)msb << 12) |
           ((int32_t)lsb << 4)  |
           ((int32_t)xlsb >> 4);
}

/* Read Raw Pressure */
int32_t bmp280_read_raw_pressure(void)
{
    uint8_t msb  = bmp280_read_register(0xF7);
    uint8_t lsb  = bmp280_read_register(0xF8);
    uint8_t xlsb = bmp280_read_register(0xF9);

    return ((int32_t)msb << 12) |
           ((int32_t)lsb << 4)  |
           ((int32_t)xlsb >> 4);
}

/* Compensate Temperature */
float bmp280_compensate_temp(int32_t raw_temp)
{
    int32_t var1, var2, T;
    
    var1 = ((((raw_temp >> 3) - ((int32_t)calib_data.dig_T1 << 1))) * 
            ((int32_t)calib_data.dig_T2)) >> 11;
    
    var2 = (((((raw_temp >> 4) - ((int32_t)calib_data.dig_T1)) * 
              ((raw_temp >> 4) - ((int32_t)calib_data.dig_T1))) >> 12) * 
            ((int32_t)calib_data.dig_T3)) >> 14;
    
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    
    return (float)T / 100.0f;
}

/* Compensate Pressure */
float bmp280_compensate_pressure(int32_t raw_press)
{
    int64_t var1, var2, p;
    
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)calib_data.dig_P6;
    var2 = var2 + ((var1 * (int64_t)calib_data.dig_P5) << 17);
    var2 = var2 + (((int64_t)calib_data.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)calib_data.dig_P3) >> 8) +
           ((var1 * (int64_t)calib_data.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)calib_data.dig_P1) >> 33;
    
    if (var1 == 0) {
        return 0.0f; // Avoid division by zero
    }
    
    p = 1048576 - raw_press;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)calib_data.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)calib_data.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)calib_data.dig_P7) << 4);
    
    return (float)p / 256.0f;
}

void app_main(void)
{
    bmp280_spi_init();

    vTaskDelay(pdMS_TO_TICKS(100));

    uint8_t chip_id = bmp280_read_register(BMP280_CHIP_ID_REG);

    printf("BMP280 Chip ID = 0x%02X\n", chip_id);

    if(chip_id != 0x58)
    {
        printf("BMP280 NOT FOUND!\n");
        return;
    }

    printf("BMP280 DETECTED\n");
    
    // Read calibration data from the sensor
    bmp280_read_calibration_data();

    // Configure sensor for normal mode
    // 0x27 = 0b00100111: 
    // Oversampling pressure x8, oversampling temp x8, normal mode
    bmp280_write_register(0xF4, 0x27);
    
    // 0xA0 = 0b10100000:
    // Standby time 1000ms, filter coefficient 4
    bmp280_write_register(0xF5, 0xA0);
    
    vTaskDelay(pdMS_TO_TICKS(200)); // Allow time for first measurement

    while(1)
    {
        int32_t raw_temp = bmp280_read_raw_temp();
        int32_t raw_press = bmp280_read_raw_pressure();

        float temp = bmp280_compensate_temp(raw_temp);
        float pressure = bmp280_compensate_pressure(raw_press);

        printf("---------------------------------\n");
        printf("Temperature : %.2f C\n", temp);
        printf("Pressure    : %.2f hPa\n", pressure);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}