/*
 * ssd1306.h
 * SSD1306 OLED Driver Header
 */

#ifndef SSD1306_H_
#define SSD1306_H_

#include "stm32_f446xx.h"

// I2C Address
#define SSD1306_I2C_ADDR        0x78U

// Screen Dimensions
#define SSD1306_WIDTH           128U
#define SSD1306_HEIGHT          64U

// Commands
#define SSD1306_CMD_DISPLAY_OFF 0xAEU
#define SSD1306_CMD_DISPLAY_ON  0xAFU
#define SSD1306_CMD_SET_MEM_ADDR_MODE 0x20U
#define SSD1306_CMD_SET_PAGE_START    0xB0U
#define SSD1306_CMD_COM_SCAN_DEC      0xC8U
#define SSD1306_CMD_SET_LOW_COL       0x00U
#define SSD1306_CMD_SET_HIGH_COL      0x10U
#define SSD1306_CMD_SET_START_LINE    0x40U
#define SSD1306_CMD_SET_CONTRAST      0x81U
#define SSD1306_CMD_SEG_REMAP         0xA1U
#define SSD1306_CMD_NORMAL_DISPLAY    0xA6U
#define SSD1306_CMD_SET_MUX_RATIO     0xA8U
#define SSD1306_CMD_DISPLAY_ALL_ON_RESUME 0xA4U
#define SSD1306_CMD_SET_DISPLAY_OFFSET 0xD3U
#define SSD1306_CMD_SET_Display_CLK_DIV 0xD5U
#define SSD1306_CMD_SET_PRECHARGE     0xD9U
#define SSD1306_CMD_SET_COM_PINS      0xDAU
#define SSD1306_CMD_SET_VCOMH_DESEL   0xDBU
#define SSD1306_CMD_CHARGE_PUMP       0x8DU

void SSD1306_Init(void);
void SSD1306_Clear(void);
void SSD1306_Update(void);
void SSD1306_SetCursor(uint8_t x, uint8_t y);
void SSD1306_Print(char* str);
void SSD1306_PrintNumber(int num);
void SSD1306_DrawChar(char ch);
void SSD1306_PrintCentered(uint8_t y, char* str);

#endif /* SSD1306_H_ */
