/*
 * fonts.h
 * Font Library Header
 */

#ifndef FONTS_H_
#define FONTS_H_

#include "stm32_f446xx.h"    // Include hardware definitions to get uint8_t types

// External declaration of the 5x7 font array
// MISRA: Explicit size is better if known, but [] is standard for flexible arrays.
// This array contains pixel data for 96 ASCII characters, 5 bytes per character.
extern const uint8_t Font5x7[480]; 

#endif /* FONTS_H_ */
