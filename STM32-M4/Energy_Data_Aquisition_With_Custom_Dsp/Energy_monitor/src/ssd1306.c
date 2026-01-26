/*
 * ssd1306.c
 * SSD1306 OLED Driver
 */

#include "ssd1306.h"
#include "i2c_driver.h"
#include "fonts.h"
#include <string.h>

#define SSD1306_ADDR 0x78

static uint8_t OLED_Buffer[1024];
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

void SSD1306_Init(void) {
    uint8_t cmds[] = { 
        0xAE,0x20,0x10,0xB0,0xC8,0x00,0x10,0x40,0x81,0xFF,0xA1,0xA6,
        0xA8,0x3F,0xA4,0xD3,0x00,0xD5,0xF0,0xD9,0x22,0xDA,0x12,0xDB,
        0x20,0x8D,0x14,0xAF 
    };
    for(int i=0; i<sizeof(cmds); i++) I2C1_Write(SSD1306_ADDR, 0x00, cmds[i]);
}

void SSD1306_Clear(void) {
    memset(OLED_Buffer, 0, 1024);
}

void SSD1306_Update(void) {
    for(int i=0; i<8; i++) {
        I2C1_Write(SSD1306_ADDR, 0x00, 0xB0+i);
        I2C1_Write(SSD1306_ADDR, 0x00, 0x00);
        I2C1_Write(SSD1306_ADDR, 0x00, 0x10);
        I2C1_WriteMulti(SSD1306_ADDR, 0x40, &OLED_Buffer[128*i], 128);
    }
}

void SSD1306_SetCursor(uint8_t x, uint8_t y) {
    cursor_x = x;
    cursor_y = y;
}

void SSD1306_DrawChar(char ch) {
    if (cursor_x >= 123) return;
    int idx = ch - 32;
    if (idx < 0 || idx > 95) idx = 0;
    
    for (int i=0; i<5; i++) {
        OLED_Buffer[cursor_x++ + (cursor_y * 128)] = Font5x7[(idx * 5) + i];
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
    
    // Snippet logic handles negative check manually or assumes?
    // Snippet had: if(num==0)... while(num>0)... while(--i>=0)...
    // Ah, my decomposed `energy_meter.c` logic already covers this or expects this.
    // Wait, snippet `SSD1306_PrintNumber`:
    /*
    void SSD1306_PrintNumber(int num) {
        char buf[10]; int i=0;
        if(num==0) { SSD1306_Print("0"); return; }
        while(num>0) { buf[i++]=(num%10)+'0'; number/=10; } // Typo in snippet: number/=10
    }
    */
    // Snippet HAS A TYPO: `number` vs `num` inside `SSD1306_PrintNumber`.
    // I must correct it to `num`.
    // Also, snippet in Step 347 `SSD1306_PrintNumber` body:
    // `while(num>0) { buf[i++]=(num%10)+'0'; num/=10; }`
    // It does NOT handle negative numbers.
    // I will stick to my robust implementation or exact snippet?
    // User said "without arror". Sending negative numbers to this snippet function would loop forever? No, `num` becomes neg?
    // `while(num>0)` would be false immediately for negative. Prints nothing.
    // I will fix the typo and add negative handling to be safe/correct as per my previous versions.
    
    if(num < 0) {
        SSD1306_Print("-");
        num = -num;
    }
    
    while(num > 0) { 
        buf[i++] = (num % 10) + '0'; 
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
    
    SSD1306_SetCursor(x, y);
    SSD1306_Print(str);
}
