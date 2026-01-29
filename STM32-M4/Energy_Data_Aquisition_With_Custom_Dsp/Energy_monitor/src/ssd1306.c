/*
 * ssd1306.c
 * SSD1306 OLED Display Driver Implementation
 * MISRA Refactored
 */

#include "ssd1306.h"
#include "i2c_driver.h"
#include "fonts.h"
#include <string.h>

// Frame Buffer
static uint8_t OLED_Buffer[1024]; 

// Cursor Position
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

/*
 * @brief  Initializes the SSD1306 OLED Display
 */
void SSD1306_Init(void) {
    uint8_t cmds[] = { 
        SSD1306_CMD_DISPLAY_OFF,
        SSD1306_CMD_SET_MEM_ADDR_MODE, 0x10U,   // Page Addressing Mode
        SSD1306_CMD_SET_PAGE_START,
        SSD1306_CMD_COM_SCAN_DEC,
        SSD1306_CMD_SET_LOW_COL,
        SSD1306_CMD_SET_HIGH_COL,
        SSD1306_CMD_SET_START_LINE,
        SSD1306_CMD_SET_CONTRAST, 0xFFU,
        SSD1306_CMD_SEG_REMAP,
        SSD1306_CMD_NORMAL_DISPLAY,
        SSD1306_CMD_SET_MUX_RATIO, 0x3FU,
        SSD1306_CMD_DISPLAY_ALL_ON_RESUME,
        SSD1306_CMD_SET_DISPLAY_OFFSET, 0x00U,
        SSD1306_CMD_SET_Display_CLK_DIV, 0xF0U,
        SSD1306_CMD_SET_PRECHARGE, 0x22U,
        SSD1306_CMD_SET_COM_PINS, 0x12U,
        SSD1306_CMD_SET_VCOMH_DESEL, 0x20U,
        SSD1306_CMD_CHARGE_PUMP, 0x14U,
        SSD1306_CMD_DISPLAY_ON
    };
    
    for(uint32_t i=0; i<sizeof(cmds); i++) {
        I2C1_Write(SSD1306_I2C_ADDR, 0x00U, cmds[i]);
    }
}

/*
 * @brief  Clears the local frame buffer
 */
void SSD1306_Clear(void) {
    memset(OLED_Buffer, 0, 1024);
}

/*
 * @brief  Updates the physical display
 */
void SSD1306_Update(void) {
    for(uint8_t i=0; i<8; i++) {
        I2C1_Write(SSD1306_I2C_ADDR, 0x00U, 0xB0U + i);
        I2C1_Write(SSD1306_I2C_ADDR, 0x00U, 0x00U);
        I2C1_Write(SSD1306_I2C_ADDR, 0x00U, 0x10U);
        
        I2C1_WriteMulti(SSD1306_I2C_ADDR, 0x40U, &OLED_Buffer[128U * (uint32_t)i], 128U);
    }
}

void SSD1306_SetCursor(uint8_t x, uint8_t y) {
    cursor_x = x;
    cursor_y = y;
}

void SSD1306_DrawChar(char ch) {
    if (cursor_x >= 123U) return;
    
    int idx = (int)ch - 32; 
    if (idx < 0 || idx > 95) idx = 0;
    
    for (int i=0; i<5; i++) {
        uint32_t buffer_index = (uint32_t)cursor_x + ((uint32_t)cursor_y * 128U);
        OLED_Buffer[buffer_index] = Font5x7[(idx * 5) + i];
        cursor_x++;
    }
    cursor_x++;
}

void SSD1306_Print(char* str) {
    while (*str) {
        SSD1306_DrawChar(*str++);
    }
}

void SSD1306_PrintNumber(int num) {
    char buf[10];
    int i = 0;
    
    if(num == 0) { 
        SSD1306_Print("0"); 
        return; 
    }
    
    if(num < 0) { 
        SSD1306_Print("-");
        num = -num;
    }
    
    while(num > 0) { 
        buf[i++] = (char)((num % 10) + '0'); 
        num /= 10; 
    }
    
    while(--i >= 0) {
        SSD1306_DrawChar(buf[i]);
    }
}

void SSD1306_PrintCentered(uint8_t y, char* str) {
    int len = 0; 
    char* p = str;
    
    while(*p++) len++;
    
    int x = (128 - (len * 6)) / 2;
    if(x < 0) x = 0;
    
    SSD1306_SetCursor((uint8_t)x, y);
    SSD1306_Print(str);
}
