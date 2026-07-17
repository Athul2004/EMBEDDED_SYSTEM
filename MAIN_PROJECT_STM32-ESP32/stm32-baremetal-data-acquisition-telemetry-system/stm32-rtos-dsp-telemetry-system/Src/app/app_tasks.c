#include "app/app_tasks.h"
#include "rtos/task.h"
#include "rtos/kernel.h"
#include "sensors/bmp280.h"
#include "esp32-telemetry/spi_telemetry.h"
#include "gsm/gsm.h"
#include "watchdog/stm32f446xx_iwdg_driver.h"
#include "drivers/delay.h"
#include "rtos/idle.h"
#include "AppConfig.h"
#include <stdio.h>

volatile uint8_t trigger_sms_from_dashboard = 0;
DSP_Engine_t dsp;

static float raw_temperature = 0.0f, raw_pressure = 0.0f, raw_altitude = 0.0f;
static float filt_temperature = 0.0f, filt_pressure = 0.0f, filt_altitude = 0.0f;

#define PRINT_FLOAT(val) (int)(val), (int)(((val) - (int)(val)) * 100 < 0 ? -(((val) - (int)(val)) * 100) : (((val) - (int)(val)) * 100))

static void Task_SensorAcquisition(void) {
    while (1) {
        BMP280_ReadData(&raw_temperature, &raw_pressure, &raw_altitude);
        Task_Delay(APP_SENSOR_READ_INTERVAL_MS);
    }
}

static void Task_DSP_Processing(void) {
    while (1) {
        filt_temperature = raw_temperature;
        filt_pressure = raw_pressure;
        filt_altitude = raw_altitude;
        
        DSP_Process(&dsp, &filt_temperature, &filt_pressure, &filt_altitude, 0.1f);
        Task_Delay(APP_DSP_PROCESS_INTERVAL_MS);
    }
}

static void Task_Telemetry_SPI(void) {
    while (1) {
        Telemetry_Send(filt_temperature, filt_pressure, filt_altitude, dsp.vertical_speed);
        Task_Delay(APP_TELEMETRY_SPI_INTERVAL_MS);
    }
}

static void Task_Telemetry_UART(void) {
    while (1) {
        printf("[RAW]  Temp: %d.%02d C | Press: %d.%02d Pa | Alt: %d.%02d m\r\n", 
               PRINT_FLOAT(raw_temperature), PRINT_FLOAT(raw_pressure), PRINT_FLOAT(raw_altitude));
               
        printf("[FILT] Temp: %d.%02d C | Press: %d.%02d Pa | Alt: %d.%02d m | Vert Speed: %d.%02d m/s\r\n\r\n", 
               PRINT_FLOAT(filt_temperature), PRINT_FLOAT(filt_pressure), PRINT_FLOAT(filt_altitude), PRINT_FLOAT(dsp.vertical_speed));
        Task_Delay(APP_TELEMETRY_UART_INTERVAL_MS);
    }
}

static void Task_GSM_Handler(void) {
    static int sms_cooldown = 0;
    while (1) {
        if (sms_cooldown > 0) sms_cooldown--;
        
        TelemetryData_t currentData = {
            .temperature = filt_temperature,
            .pressure = filt_pressure,
            .altitude = filt_altitude,
            .vertical_speed = dsp.vertical_speed
        };
        GSM_CheckButtonAndSendSMS(APP_TARGET_PHONE_NUMBER, &currentData);
        
        if (trigger_sms_from_dashboard) {
            trigger_sms_from_dashboard = 0;
            if (sms_cooldown == 0) {
                Task_Delay(100); 
                printf(">>> SMS TRIGGER RECEIVED FROM MQTT DASHBOARD! <<<\r\n");
                GSM_SendSMS(APP_TARGET_PHONE_NUMBER, &currentData);
                sms_cooldown = 100; // 100 * 50ms = 5000ms cooldown
            }
        }
        
        Task_Delay(APP_GSM_HANDLER_INTERVAL_MS);
    }
}

static void Task_Watchdog(void) {
    while (1) {
        IWDG_Refresh();
        Task_Delay(APP_WATCHDOG_INTERVAL_MS);
    }
}

// Example Heartbeat Task
static void Task_ExampleHeartbeat(void) {
    while (1) {
        printf("[HEARTBEAT] System is alive and running!\r\n");
        Task_Delay(APP_EXAMPLE_HEARTBEAT_MS);
    }
}

void AppTasks_Init(void) {
    Task_Create(Task_Watchdog, APP_PRIORITY_WATCHDOG);         // Highest priority
    Task_Create(Task_SensorAcquisition, APP_PRIORITY_SENSOR_ACQ);
    Task_Create(Task_DSP_Processing, APP_PRIORITY_DSP);
    Task_Create(Task_GSM_Handler, APP_PRIORITY_GSM);
    Task_Create(Task_Telemetry_SPI, APP_PRIORITY_TELEMETRY_SPI);
    Task_Create(Task_Telemetry_UART, APP_PRIORITY_TELEMETRY_UART);
    Task_Create(Task_ExampleHeartbeat, APP_PRIORITY_EXAMPLE);  // Lowest priority
    Idle_Init();
}
