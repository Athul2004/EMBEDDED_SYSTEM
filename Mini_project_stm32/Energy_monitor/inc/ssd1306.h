/*
 * ssd1306.h
 * SSD1306 OLED Driver Header
 */

#ifndef SSD1306_H_
#define SSD1306_H_

#include "stm32_f446xx.h"    // Include hardware definitions

// I2C Address for the OLED display
#define SSD1306_I2C_ADDR        0x78U

// Screen Dimensions in pixels
#define SSD1306_WIDTH           128U
#define SSD1306_HEIGHT          64U

// SSD1306 Commands Map
#define SSD1306_CMD_DISPLAY_OFF 0xAEU       // Turn display OFF
#define SSD1306_CMD_DISPLAY_ON  0xAFU       // Turn display ON
#define SSD1306_CMD_SET_MEM_ADDR_MODE 0x20U // Set Memory Addressing Mode
#define SSD1306_CMD_SET_PAGE_START    0xB0U // Set Page Start Address for Page Addressing Mode
#define SSD1306_CMD_COM_SCAN_DEC      0xC8U // Set COM Output Scan Direction (Remapped)
#define SSD1306_CMD_SET_LOW_COL       0x00U // Set Lower Column Start Address for Page Addressing Mode
#define SSD1306_CMD_SET_HIGH_COL      0x10U // Set Higher Column Start Address for Page Addressing Mode
#define SSD1306_CMD_SET_START_LINE    0x40U // Set Display Start Line
#define SSD1306_CMD_SET_CONTRAST      0x81U // Set Contrast Control
#define SSD1306_CMD_SEG_REMAP         0xA1U // Set Segment Re-map
#define SSD1306_CMD_NORMAL_DISPLAY    0xA6U // Set Normal Display (Inverse Off)
#define SSD1306_CMD_SET_MUX_RATIO     0xA8U // Set Multiplex Ratio
#define SSD1306_CMD_DISPLAY_ALL_ON_RESUME 0xA4U // Entire Display ON -> Resume to RAM content
#define SSD1306_CMD_SET_DISPLAY_OFFSET 0xD3U // Set Display Offset
#define SSD1306_CMD_SET_Display_CLK_DIV 0xD5U // Set Display Clock Divide Ratio/Oscillator Frequency
#define SSD1306_CMD_SET_PRECHARGE     0xD9U // Set Pre-charge Period
#define SSD1306_CMD_SET_COM_PINS      0xDAU // Set COM Pins Hardware Configuration
#define SSD1306_CMD_SET_VCOMH_DESEL   0xDBU // Set VCOMH Deselect Level
#define SSD1306_CMD_CHARGE_PUMP       0x8DU // Charge Pump Setting

// Initializes the SSD1306 display via I2C
void SSD1306_Init(void);

// Clears the internal display buffer
void SSD1306_Clear(void);

// Sends the internal buffer to the display to update it
void SSD1306_Update(void);

// Sets the cursor position for text rendering (x: 0-127, y: 0-7 pages)
void SSD1306_SetCursor(uint8_t x, uint8_t y);

// Prints a string to the display at the current cursor position
void SSD1306_Print(char* str);

// Prints a number to the display
void SSD1306_PrintNumber(int num);

// Draws a single character to the internal buffer
void SSD1306_DrawChar(char ch);

// Prints a string centered on a specific line (y: page number)
void SSD1306_PrintCentered(uint8_t y, char* str);

#endif /* SSD1306_H_ */
