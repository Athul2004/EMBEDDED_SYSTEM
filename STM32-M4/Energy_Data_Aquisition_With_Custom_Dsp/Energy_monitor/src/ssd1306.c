/*
 * ssd1306.c
 * SSD1306 OLED Display Driver Implementation
 * MISRA Refactored
 */

#include "ssd1306.h"    // Include driver header
#include "i2c_driver.h" // Include I2C driver for communication
#include "fonts.h"      // Include font definition
#include <string.h>     // Include string library for memset

// Frame Buffer
// 128 columns * 64 rows = 8192 bits = 1024 bytes
// This buffer holds the pixel data locally before sending to display
static uint8_t OLED_Buffer[1024]; 

// Cursor Position
// Tracks the current X (column) and Y (page) for text printing
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

/*
 * @brief  Initializes the SSD1306 OLED Display
 */
void SSD1306_Init(void) {
    // Initialization Sequence Command Array
    uint8_t cmds[] = { 
        SSD1306_CMD_DISPLAY_OFF,          // Turn Display OFF
        SSD1306_CMD_SET_MEM_ADDR_MODE, 0x10U,   // Set Memory Addressing Mode to Page Addressing
        SSD1306_CMD_SET_PAGE_START,       // Set Page Start Address (Page 0)
        SSD1306_CMD_COM_SCAN_DEC,         // Set COM Scan Direction (Remapped)
        SSD1306_CMD_SET_LOW_COL,          // Set Lower Column Start Address
        SSD1306_CMD_SET_HIGH_COL,         // Set Higher Column Start Address
        SSD1306_CMD_SET_START_LINE,       // Set Display Start Line
        SSD1306_CMD_SET_CONTRAST, 0xFFU,  // Set Contrast Control (Max 0xFF)
        SSD1306_CMD_SEG_REMAP,            // Set Segment Re-map (Col 127 mapped to SEG0)
        SSD1306_CMD_NORMAL_DISPLAY,       // Set Normal Display (Not Inverted)
        SSD1306_CMD_SET_MUX_RATIO, 0x3FU, // Set Multiplex Ratio (1/64 duty)
        SSD1306_CMD_DISPLAY_ALL_ON_RESUME,// Resume to RAM content display
        SSD1306_CMD_SET_DISPLAY_OFFSET, 0x00U, // Set Display Offset (0)
        SSD1306_CMD_SET_Display_CLK_DIV, 0xF0U, // Set Display Clock Divide Ratio
        SSD1306_CMD_SET_PRECHARGE, 0x22U, // Set Pre-charge Period
        SSD1306_CMD_SET_COM_PINS, 0x12U,  // Set COM Pins Config (Alternative, Non-Remapped)
        SSD1306_CMD_SET_VCOMH_DESEL, 0x20U, // Set VCOMH Deselect level
        SSD1306_CMD_CHARGE_PUMP, 0x14U,   // Enable Charge Pump (Internal VCC)
        SSD1306_CMD_DISPLAY_ON            // Turn Display ON
    };
    
    // Loop through command array and send each byte via I2C
    // Register 0x00 is Command Register
    for(uint32_t i=0; i<sizeof(cmds); i++) {
        I2C1_Write(SSD1306_I2C_ADDR, 0x00U, cmds[i]);
    }
}

/*
 * @brief  Clears the local frame buffer
 */
void SSD1306_Clear(void) {
    // Fill buffer with 0x00 (all pixels off)
    memset(OLED_Buffer, 0, 1024);
}

/*
 * @brief  Updates the physical display
 */
void SSD1306_Update(void) {
    // Iterate through all 8 pages (rows of 8 pixels height)
    for(uint8_t i=0; i<8; i++) {
        // Set Page Address (0xB0 to 0xB7)
        I2C1_Write(SSD1306_I2C_ADDR, 0x00U, 0xB0U + i);
        // Set Lower Column Start Address (0x00)
        I2C1_Write(SSD1306_I2C_ADDR, 0x00U, 0x00U);
        // Set Higher Column Start Address (0x10)
        I2C1_Write(SSD1306_I2C_ADDR, 0x00U, 0x10U);
        
        // Write the data for this page (128 bytes)
        // Register 0x40 is Data Register (Co=0, D/C#=1)
        I2C1_WriteMulti(SSD1306_I2C_ADDR, 0x40U, &OLED_Buffer[128U * (uint32_t)i], 128U);
    }
}

/*
 * @brief  Sets the cursor position
 * @param  x: Column (0-127)
 * @param  page: Page (0-7)
 */
void SSD1306_SetCursor(uint8_t x, uint8_t y) {
    cursor_x = x;
    cursor_y = y;
}

/*
 * @brief  Draws a character to buffer
 * @param  ch: Character to draw
 */
void SSD1306_DrawChar(char ch) {
    // Check bounds: if too close to right edge, don't draw
    if (cursor_x >= 123U) return;
    
    // Calculate index in Font array. ASCII Offset is 32 (Space).
    int idx = (int)ch - 32; 
    // Boundary check for supported characters
    if (idx < 0 || idx > 95) idx = 0;
    
    // Copy 5 bytes of font data to buffer
    for (int i=0; i<5; i++) {
        // Calculate buffer index: current_x + (page * 128)
        uint32_t buffer_index = (uint32_t)cursor_x + ((uint32_t)cursor_y * 128U);
        OLED_Buffer[buffer_index] = Font5x7[(idx * 5) + i];
        cursor_x++; // Advance cursor
    }
    cursor_x++; // Add 1 pixel spacing between characters
}

/*
 * @brief  Prints a string
 */
void SSD1306_Print(char* str) {
    while (*str) {
        SSD1306_DrawChar(*str++); // Draw char and advance pointer
    }
}

/*
 * @brief  Prints a number
 */
void SSD1306_PrintNumber(int num) {
    char buf[10]; // Buffer for string representation
    int i = 0;
    
    if(num == 0) { 
        SSD1306_Print("0"); 
        return; 
    }
    
    if(num < 0) { 
        SSD1306_Print("-");
        num = -num;
    }
    
    // Convert generic integer to string (reverse order)
    while(num > 0) { 
        buf[i++] = (char)((num % 10) + '0'); 
        num /= 10; 
    }
    
    // Print in correct order
    while(--i >= 0) {
        SSD1306_DrawChar(buf[i]);
    }
}

/*
 * @brief  Prints a string centered on a line
 */
void SSD1306_PrintCentered(uint8_t y, char* str) {
    int len = 0; 
    char* p = str;
    
    // Calculate string length
    while(*p++) len++;
    
    // Calculate X start position: (Screen Width - (Len * CharWidth)) / 2
    // Char width is 6 (5 pixels + 1 spacing)
    int x = (128 - (len * 6)) / 2;
    if(x < 0) x = 0;
    
    SSD1306_SetCursor((uint8_t)x, y);
    SSD1306_Print(str);
}
