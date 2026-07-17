#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// Networking headers
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "mqtt_server.h"
#include "sms_controller.h"

#define WIFI_SSID "1-SAROVARAM-4G_EXT"
#define WIFI_PASS "Athul162004@"

static const char *TAG = "APP_MAIN";

// Global socket descriptor
int mqtt_sock = -1;

extern void spi_slave_task(void *pvParameters);

static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        ESP_LOGI(TAG, "Retry to connect to the AP");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        
        // Connect to HiveMQ broker
        struct hostent *hp = gethostbyname("broker.hivemq.com");
        if (!hp) {
            ESP_LOGE(TAG, "DNS lookup failed");
            return;
        }

        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(1883);
        server_addr.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;

        mqtt_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (mqtt_sock < 0) {
            ESP_LOGE(TAG, "Failed to allocate socket");
            return;
        }

        if (connect(mqtt_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) {
            ESP_LOGE(TAG, "Socket connect failed");
            close(mqtt_sock);
            mqtt_sock = -1;
            return;
        }

        ESP_LOGI(TAG, "Connected to HiveMQ Broker!");

        // Send MQTT CONNECT Packet
        mqtt_connect_opt_t conn_opts = {
            .client_id = "ESP32_BMP280_Device",
            .username = NULL,
            .password = NULL,
            .keep_alive = 60
        };

        uint8_t buffer[128];
        int len = mqtt_build_connect_packet(buffer, sizeof(buffer), &conn_opts);
        if (len > 0) {
            send(mqtt_sock, buffer, len, 0);
            ESP_LOGI(TAG, "Sent MQTT CONNECT packet");
        }
        
        // Wait briefly for broker to accept connection before subscribing
        vTaskDelay(pdMS_TO_TICKS(100));

        // Send MQTT SUBSCRIBE Packet
        uint8_t sub_buffer[128];
        int sub_len = mqtt_build_subscribe_packet(sub_buffer, sizeof(sub_buffer), "command/sms", 1);
        if (sub_len > 0) {
            send(mqtt_sock, sub_buffer, sub_len, 0);
            ESP_LOGI(TAG, "Sent MQTT SUBSCRIBE packet for command/sms");
        }
    }
}

void wifi_init_sta(void) {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, &instance_any_id);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, &instance_got_ip);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();

    ESP_LOGI(TAG, "wifi_init_sta finished.");
}

void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "Starting ESP32 Bare-Metal Drivers Examples with Wi-Fi & MQTT...");

    // Initialize Wi-Fi
    wifi_init_sta();

    // Start SMS command listener task
    sms_controller_start();

    // 4. SPI Slave Receive Example
    xTaskCreate(spi_slave_task, "spi_slave_ex", 8192, NULL, 5, NULL);
}
