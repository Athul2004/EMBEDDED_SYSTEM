#include "sms_controller.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <sys/socket.h>
#include <string.h>

extern int mqtt_sock;

volatile bool sms_trigger_pending = false;
static const char *TAG = "SMS_CTRL";

static void mqtt_rx_task(void *pvParameters) {
    ESP_LOGI(TAG, "SMS Controller RX task started");
    uint8_t rx_buffer[256];
    
    while (1) {
        if (mqtt_sock >= 0) {
            int len = recv(mqtt_sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
            if (len > 0) {
                bool found = false;
                const char *target = "trigger_sms";
                int target_len = strlen(target);
                for (int i = 0; i <= len - target_len; i++) {
                    if (memcmp(&rx_buffer[i], target, target_len) == 0) {
                        found = true;
                        break;
                    }
                }
                
                if (found) {
                    ESP_LOGI(TAG, "Received SMS trigger from MQTT dashboard!");
                    sms_trigger_pending = true;
                }
            } else {
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
        } else {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        
        // Yield
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void sms_controller_start(void) {
    xTaskCreate(mqtt_rx_task, "mqtt_rx_task", 4096, NULL, 5, NULL);
}
