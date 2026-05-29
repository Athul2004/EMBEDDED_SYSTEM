#include "stm32_f446xx.h"
#include "i2c.h"
#include "uart.h"

#define DS1307_ADDR 0xD0

void delay(void)
{
    for(volatile uint32_t i = 0; i < 1000000; i++);
}

uint8_t BCD_To_Dec(uint8_t value)
{
    return (((value >> 4) * 10) + (value & 0x0F));
}

void UART_PrintHex(uint8_t value)
{
    char hex[] = "0123456789ABCDEF";

    UART2_WriteChar(hex[(value >> 4) & 0x0F]);
    UART2_WriteChar(hex[value & 0x0F]);
}

int main(void)
{
    uint8_t sec_raw;
    uint8_t sec;

    UART2_Init();
    I2C1_Init();

    UART2_WriteString("\r\n");
    UART2_WriteString("=================================\r\n");
    UART2_WriteString("STM32F446RE + DS1307 RTC TEST\r\n");
    UART2_WriteString("=================================\r\n");

    while(1)
    {
        I2C1_Read(DS1307_ADDR, 0x00, &sec_raw);

        UART2_WriteString("RAW = ");
        UART_PrintHex(sec_raw);

        sec = BCD_To_Dec(sec_raw & 0x7F);

        UART2_WriteString("   Seconds = ");

        UART2_WriteChar((sec / 10) + '0');
        UART2_WriteChar((sec % 10) + '0');

        UART2_WriteString("\r\n");

        delay();
    }
}
