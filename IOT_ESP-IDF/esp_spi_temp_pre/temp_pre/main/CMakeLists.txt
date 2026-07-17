idf_component_register(SRCS "temp_pre.c"
                    INCLUDE_DIRS "."
                    REQUIRES driver nvs_flash esp_event mqtt)