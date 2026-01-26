#ifndef SSD1306_H_
#define SSD1306_H_

#include "stm32_f446xx.h"

void SSD1306_Init(void);
void SSD1306_Clear(void);
void SSD1306_Update(void);
void SSD1306_SetCursor(uint8_t x, uint8_t y);
void SSD1306_Print(char* str);
void SSD1306_PrintNumber(int num);
void SSD1306_DrawChar(char ch);
void SSD1306_PrintCentered(uint8_t y, char* str);

#endif
