idf_component_register(SRCS "sms_controller.c"
                       INCLUDE_DIRS "include"
                       REQUIRES "esp_wifi" "lwip" "mqtt_server")
