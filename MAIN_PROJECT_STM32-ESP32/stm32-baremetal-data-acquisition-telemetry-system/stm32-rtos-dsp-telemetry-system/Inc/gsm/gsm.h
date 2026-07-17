#ifndef GSM_H
#define GSM_H

#include "drivers/stm32f446xx.h"
#include "drivers/stm32f446xx_gpio_driver.h"
#include "drivers/stm32f446xx_usart_driver.h"

// Telemetry data structure
typedef struct {
    float temperature;
    float pressure;
    float altitude;
    float vertical_speed;
} TelemetryData_t;

// Function prototypes
void GSM_Button_Init(void);
void GSM_UART_Init(void);
void GSM_SendSMS(const char *phoneNumber, TelemetryData_t *data);
void GSM_CheckButtonAndSendSMS(const char *phoneNumber, TelemetryData_t *data);

#endif /* GSM_H */
