/*
 * fonts.h
 * Font Library Header
 */

#ifndef FONTS_H_
#define FONTS_H_

#include "stm32_f446xx.h"

// External declaration of the 5x7 font array
// MISRA: Explicit size is better if known, but [] is standard for flexible arrays.
extern const uint8_t Font5x7[480]; // 96 chars * 5 bytes

#endif /* FONTS_H_ */
