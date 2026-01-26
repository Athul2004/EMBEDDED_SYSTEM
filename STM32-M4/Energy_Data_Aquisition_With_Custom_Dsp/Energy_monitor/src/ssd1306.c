/*
 * ssd1306.c
 * SSD1306 OLED Display Driver Implementation
 */

#include "ssd1306.h"            // Driver Header
#include "i2c_driver.h"         // I2C Driver for communication
#include "fonts.h"              // Font data (Font5x7)
#include <string.h>             // Memset

// --- Configuration ---
#define SSD1306_ADDR 0x78       // I2C 8-bit Address (0x3C << 1)

// --- Frame Buffer ---
// 128 Columns * 64 Rows / 8 bits per byte = 1024 Bytes
static uint8_t OLED_Buffer[1024]; 

// --- Cursor Position ---
static uint8_t cursor_x = 0;    // Current X Position (Column 0-127)
static uint8_t cursor_y = 0;    // Current Y Position (Page 0-7)

/*
 * @brief  Initializes the SSD1306 OLED Display with standard reset sequence
 * @param  None
 * @retval None
 */
void SSD1306_Init(void) {
    // Initialization Commands Sequence
    uint8_t cmds[] = { 
        0xAE,           // Display OFF (Sleep Mode)
        0x20, 0x10,     // Set Memory Addressing Mode to Page Addressing Mode (0x10)
        0xB0,           // Set Page Start Address to Page 0
        0xC8,           // Set COM Output Scan Direction (Remapped: COM[N-1] to COM0)
        0x00,           // Set Low Column Address
        0x10,           // Set High Column Address
        0x40,           // Set Start Line Address to Line 0
        0x81, 0xFF,     // Set Contrast Control (0xFF = Max Contrast)
        0xA1,           // Set Segment Re-map (Column 127 mapped to SEG0)
        0xA6,           // Set Normal Display (Not Inverse)
        0xA8, 0x3F,     // Set Multiplex Ratio to 1/64 duty (0x3F + 1 = 64)
        0xA4,           // Output follows RAM content (Entire Display ON off)
        0xD3, 0x00,     // Set Display Offset to 0
        0xD5, 0xF0,     // Set Display Clock Divide Ratio/Oscillator Frequency (Max)
        0xD9, 0x22,     // Set Pre-charge Period
        0xDA, 0x12,     // Set COM Pins Hardware Configuration
        0xDB, 0x20,     // Set V_COMH Deselect Level
        0x8D, 0x14,     // Enable Charge Pump Regulator (Required for internal VCC generation)
        0xAF            // Display ON (Wake up)
    };
    
    // Loop through and send each command byte
    // I2C Write format: [Start] [Addr] [Ctrl Byte: 0x00 for Cmd] [Cmd Byte] [Stop]
    for(int i=0; i<sizeof(cmds); i++) {
        I2C1_Write(SSD1306_ADDR, 0x00, cmds[i]);
    }
}

/*
 * @brief  Clears the local frame buffer (fills with 0)
 * @param  None
 * @retval None
 * @note   Must call Update() afterwards to see changes on screen.
 */
void SSD1306_Clear(void) {
    memset(OLED_Buffer, 0, 1024); // Set all 1024 bytes to 0x00
}

/*
 * @brief  Updates the physical display with the content of the local buffer
 * @param  None
 * @retval None
 */
void SSD1306_Update(void) {
    // Loop through all 8 pages (rows of byte-height)
    for(int i=0; i<8; i++) {
        // Prepare Page Address Setup
        I2C1_Write(SSD1306_ADDR, 0x00, 0xB0+i); // Set Page Start Address (Page i)
        I2C1_Write(SSD1306_ADDR, 0x00, 0x00);   // Set Lower Column Start Address (0)
        I2C1_Write(SSD1306_ADDR, 0x00, 0x10);   // Set Higher Column Start Address (0)
        
        // Write Data for the entire page (128 columns) in one burst
        // Ctrl Byte 0x40 indicates Data Stream follows
        I2C1_WriteMulti(SSD1306_ADDR, 0x40, &OLED_Buffer[128*i], 128);
    }
}

/*
 * @brief  Sets the text cursor position for subsequent prints
 * @param  x: Column (0-127)
 * @param  y: Page (0-7)
 * @retval None
 */
void SSD1306_SetCursor(uint8_t x, uint8_t y) {
    cursor_x = x;
    cursor_y = y;
}

/*
 * @brief  Draws a single character to the buffer at current cursor pos
 * @param  ch: ASCII character to draw
 * @retval None
 */
void SSD1306_DrawChar(char ch) {
    // Check bounds: If cursor is near end of line, abort (simple clipping)
    if (cursor_x >= 123) return; // Char width 5 + 1 space = 6 pixels
    
    // Calculate index in Font array
    // Font starts at Space (' ', 32). ASCII < 32 not supported.
    int idx = ch - 32; 
    if (idx < 0 || idx > 95) idx = 0; // Default to Space if invalid
    
    // Copy 5 columns of pixel data from Font to Buffer
    for (int i=0; i<5; i++) {
        // Calculate buffer index: current_x + (current_page * 128_width)
        OLED_Buffer[cursor_x++ + (cursor_y * 128)] = Font5x7[(idx * 5) + i];
    }
    
    cursor_x++; // Add 1 pixel spacing after character
}

/*
 * @brief  Prints a null-terminated string
 * @param  str: String to print
 * @retval None
 */
void SSD1306_Print(char* str) {
    // Loop until null terminator
    while (*str) {
        SSD1306_DrawChar(*str++);
    }
}

/*
 * @brief  Prints an integer number (Decimal)
 * @param  num: Integer value
 * @retval None
 */
void SSD1306_PrintNumber(int num) {
    char buf[10];   // Buffer for digits
    int i = 0;
    
    // Special case 0
    if(num == 0) { 
        SSD1306_Print("0"); 
        return; 
    }
    
    // Handle negative
    if(num < 0) { 
        SSD1306_Print("-");
        num = -num;
    }
    
    // Extract digits (Reverse order)
    while(num > 0) { 
        buf[i++] = (num % 10) + '0'; 
        num /= 10; 
    }
    
    // Print digits in correct order
    while(--i >= 0) {
        SSD1306_DrawChar(buf[i]);
    }
}

/*
 * @brief  Prints a string centered horizontally on a specific page
 * @param  y: Page number (0-7)
 * @param  str: String to print
 * @retval None
 */
void SSD1306_PrintCentered(uint8_t y, char* str) {
    int len = 0; 
    char* p = str;
    
    // Calculate string length
    while(*p++) len++;
    
    // Calculate X start position for centering
    // Screen Width (128) - (String Length * Char Width (6)) / 2
    int x = (128 - (len * 6)) / 2;
    if(x < 0) x = 0; // Clamp to 0 if string too long
    
    // Set position and print
    SSD1306_SetCursor(x, y);
    SSD1306_Print(str);
}
