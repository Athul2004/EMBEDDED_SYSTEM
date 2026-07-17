idf_component_register(SRCS "main.c" 
                           "../telemetry_stm32/telemetry_stm32.c"
                    INCLUDE_DIRS "."
                    REQUIRES spi_slave_driver nvs_flash esp_wifi esp_netif lwip mqtt_server sms_controller)
