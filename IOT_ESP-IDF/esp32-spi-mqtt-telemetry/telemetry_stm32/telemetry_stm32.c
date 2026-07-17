// ============================================================
// SPI Slave Bare-Metal Example
// Receives Telemetry packets over SPI
// ============================================================
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "spi_slave.h"
#include "esp_log.h"
#include <sys/socket.h>
#include "mqtt_server.h"
#include "sms_controller.h"

// Global socket defined in main.c
extern int mqtt_sock;

// ESP32 SPI Slave Pins (VSPI default)
#define GPIO_MOSI 23
#define GPIO_MISO 19
#define GPIO_SCLK 18
#define GPIO_CS   5

static const char *TAG = "SPI_SLAVE_EX";

void spi_slave_task(void *pvParameters) {
    ESP_LOGI(TAG, "Initializing Bare-Metal SPI Slave...");
    
    // Initialize SPI Slave on VSPI (SPI3) in Mode 1 (To match STM32 fix!)
    if (!SPI_Slave_Init(GPIO_MOSI, GPIO_MISO, GPIO_SCLK, GPIO_CS, 1)) {
        ESP_LOGE(TAG, "Failed to init SPI Slave");
        vTaskDelete(NULL);
    }

    ESP_LOGI(TAG, "SPI Slave Ready. Waiting for telemetry packets...");

    TelemetryPacket_t rx_packet;
    
    while (1) {
        // Blocks until a transaction is received
        if (SPI_Slave_Receive(&rx_packet)) {
            int t_int = (int)rx_packet.temperature;
            int t_frac = (int)((rx_packet.temperature - t_int) * 100);
            if (t_frac < 0) t_frac = -t_frac;

            int p_int = (int)rx_packet.pressure;
            int p_frac = (int)((rx_packet.pressure - p_int) * 100);
            if (p_frac < 0) p_frac = -p_frac;

            int a_int = (int)rx_packet.altitude;
            int a_frac = (int)((rx_packet.altitude - a_int) * 100);
            if (a_frac < 0) a_frac = -a_frac;

            int v_int = (int)rx_packet.vertical_speed;
            int v_frac = (int)((rx_packet.vertical_speed - v_int) * 100);
            if (v_frac < 0) v_frac = -v_frac;

            // Dump RAW HEX to see exactly what arrived from the wire!
            uint32_t *raw_words = (uint32_t*)&rx_packet;
            printf("[RAW HEX DUMP] W0:%08X W1:%08X W2:%08X W3:%08X W4:%08X\n", 
                   (unsigned int)raw_words[0], (unsigned int)raw_words[1], 
                   (unsigned int)raw_words[2], (unsigned int)raw_words[3], (unsigned int)raw_words[4]);

            printf("[FILT] Temp: %d.%02d C | Press: %d.%02d Pa | Alt: %d.%02d m | Vert Speed: %d.%02d m/s\r\n\r\n", 
                   t_int, t_frac, p_int, p_frac, a_int, a_frac, v_int, v_frac);
            
            // Publish to MQTT if socket is connected
            if (mqtt_sock >= 0) {
                char payload_str[256];
                float press_hpa = rx_packet.pressure / 100.0f;
                int ph_int = (int)press_hpa;
                int ph_frac = (int)((press_hpa - ph_int) * 100);
                if (ph_frac < 0) ph_frac = -ph_frac;

                int p_len = snprintf(payload_str, sizeof(payload_str), 
                    "{\"temperature\":%d.%02d,\"pressure\":%d.%02d,\"altitude\":%d.%02d,\"vertical_speed\":%d.%02d}", 
                    t_int, t_frac, ph_int, ph_frac, a_int, a_frac, v_int, v_frac);
                
                uint8_t pub_buf[512];
                int pub_len = mqtt_build_publish_packet(pub_buf, sizeof(pub_buf), "telemetry/bmp280", (uint8_t*)payload_str, p_len, 0, false);
                if (pub_len > 0) {
                    send(mqtt_sock, pub_buf, pub_len, 0);
                }
            }
            
            // Queue response for the STM32
            TelemetryPacket_t resp = {0};
            if (sms_trigger_pending) {
                resp.timestamp = 0xAA55AA55; // Magic command number for SMS
                sms_trigger_pending = false;
                ESP_LOGI(TAG, "Loaded SMS trigger response into SPI buffer");
            }
            SPI_Slave_LoadResponse(&resp);
        }
    }
}
