#include "gsm/gsm.h"
#include "drivers/delay.h"
#include "BoardConfig.h"
#include <stdio.h>
#include <string.h>

// Assume USART3 for GSM (since USART2 is used for debug and SPI1 for telemetry)
USART_Handle_t GSM_USART_Handle;

void GSM_Button_Init(void) {
    GPIO_Handle_t buttonPin;
    
    // Enable GPIO clock
    GPIO_PeriClockControl(BOARD_GSM_BUTTON_PORT, ENABLE);
    
    // Configure button pin as input
    buttonPin.pGPIOx = BOARD_GSM_BUTTON_PORT;
    buttonPin.GPIO_PinConfig.GPIO_PinNumber = BOARD_GSM_BUTTON_PIN;
    buttonPin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    buttonPin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    // Assuming a pull-up is required if the button connects to GND
    buttonPin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    
    GPIO_Init(&buttonPin);
}

void GSM_UART_Init(void) {
    // Enable GPIO clock for USART pins
    GPIO_PeriClockControl(BOARD_GSM_UART_PORT, ENABLE);
    
    GPIO_Handle_t usartPins;
    usartPins.pGPIOx = BOARD_GSM_UART_PORT;
    usartPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    usartPins.GPIO_PinConfig.GPIO_PinAltFunMode = BOARD_GSM_UART_AF;
    usartPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    usartPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    usartPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    
    // TX
    usartPins.GPIO_PinConfig.GPIO_PinNumber = BOARD_GSM_TX_PIN;
    GPIO_Init(&usartPins);
    
    // RX
    usartPins.GPIO_PinConfig.GPIO_PinNumber = BOARD_GSM_RX_PIN;
    GPIO_Init(&usartPins);
    
    // Configure USART1
    GSM_USART_Handle.pUSARTx = USART1;
    GSM_USART_Handle.USART_Config.USART_Baud = USART_STD_BAUD_9600; // SIM900 usually uses 9600 bps by default
    GSM_USART_Handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
    GSM_USART_Handle.USART_Config.USART_Mode = USART_MODE_TXRX;
    GSM_USART_Handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
    GSM_USART_Handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
    GSM_USART_Handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
    
    USART_Init(&GSM_USART_Handle);
    USART_PeripheralControl(USART1, ENABLE);
}

static void GSM_SendString(char *str) {
    USART_SendData(&GSM_USART_Handle, (uint8_t*)str, strlen(str));
}

void GSM_SendSMS(const char *phoneNumber, TelemetryData_t *data) {
    char buffer[256];
    
    // Set SMS format to Text mode
    GSM_SendString("AT+CMGF=1\r\n");
    delay_ms(1000); // Increased delay
    
    // Set destination phone number
    sprintf(buffer, "AT+CMGS=\"%s\"\r\n", phoneNumber);
    GSM_SendString(buffer);
    delay_ms(1000); // Give module time to return the '>' prompt
    
    // Message body with float manual conversion (baremetal safe)
    int temp_int = (int)data->temperature;
    int temp_frac = (int)((data->temperature - temp_int) * 100);
    if(temp_frac < 0) temp_frac = -temp_frac;
    
    float press_hpa = data->pressure / 100.0f;
    int press_int = (int)press_hpa;
    int press_frac = (int)((press_hpa - press_int) * 100);
    if(press_frac < 0) press_frac = -press_frac;
    
    int alt_int = (int)data->altitude;
    int alt_frac = (int)((data->altitude - alt_int) * 100);
    if(alt_frac < 0) alt_frac = -alt_frac;

    int vspeed_int = (int)data->vertical_speed;
    int vspeed_frac = (int)((data->vertical_speed - vspeed_int) * 100);
    if(vspeed_frac < 0) vspeed_frac = -vspeed_frac;

    sprintf(buffer, "=== SENSOR REPORT ===\nTemp   : %d.%02d C\nPress  : %d.%02d hPa\nAlt    : %d.%02d m\nV-Speed: %d.%02d m/s\n=====================",
            temp_int, temp_frac, press_int, press_frac, alt_int, alt_frac, vspeed_int, vspeed_frac);
    
    GSM_SendString(buffer);
    delay_ms(500);
    
    // Send Ctrl+Z (0x1A) to send the SMS
    char ctrl_z = 0x1A;
    USART_SendData(&GSM_USART_Handle, (uint8_t*)&ctrl_z, 1);
    
    // Give module time to send the message
    delay_ms(5000); 
}

void GSM_CheckButtonAndSendSMS(const char *phoneNumber, TelemetryData_t *data) {
    // Read the button state
    // Assuming active LOW (button connects pin to GND when pressed)
    if (GPIO_ReadFromInputPin(BOARD_GSM_BUTTON_PORT, BOARD_GSM_BUTTON_PIN) == 0) {
        // Debounce delay
        delay_ms(50);
        if (GPIO_ReadFromInputPin(BOARD_GSM_BUTTON_PORT, BOARD_GSM_BUTTON_PIN) == 0) {
            
            printf("Button Pressed! Sending SMS to %s...\r\n", phoneNumber);
            
            // Send the SMS
            GSM_SendSMS(phoneNumber, data);
            
            printf("SMS Sent Successfully!\r\n");
            
            // Wait for button release to avoid multiple SMS on a single press
            while(GPIO_ReadFromInputPin(BOARD_GSM_BUTTON_PORT, BOARD_GSM_BUTTON_PIN) == 0) {
                delay_ms(10);
            }
        }
    }
}
