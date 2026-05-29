/*
 * stm32f44xx.h
 *
 *  Created on: 29-May-2026
 *      Author: athul
 */

#ifndef STM32F44XX_H_
#define STM32F44XX_H_

#include <stdint.h>

/* =========================
   GPIO BASE ADDRESSES
   ========================= */

#define GPIOA_BASEADDR 0x40020000U

/* =========================
   RCC BASE ADDRESS
   ========================= */

#define RCC_BASEADDR   0x40023800U

/* =========================
   USART2 BASE ADDRESS
   ========================= */

#define USART2_BASEADDR 0x40004400U

/* =========================
   GPIO REGISTER STRUCTURE
   ========================= */

typedef struct
{
    volatile uint32_t MODER;      // 0x00
    volatile uint32_t OTYPER;     // 0x04
    volatile uint32_t OSPEEDR;    // 0x08
    volatile uint32_t PUPDR;      // 0x0C
    volatile uint32_t IDR;        // 0x10
    volatile uint32_t ODR;        // 0x14
    volatile uint32_t BSRR;       // 0x18
    volatile uint32_t LCKR;       // 0x1C
    volatile uint32_t AFRL;       // 0x20
    volatile uint32_t AFRH;       // 0x24

} GPIO_RegDef_t;

/* =========================
   RCC REGISTER STRUCTURE
   ========================= */

typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;

    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    volatile uint32_t RESERVED0;

    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t RESERVED1[2];

    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;
    volatile uint32_t RESERVED2;

    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;

} RCC_RegDef_t;

/* =========================
   USART REGISTER STRUCTURE
   ========================= */

typedef struct
{
    volatile uint32_t SR;      // 0x00
    volatile uint32_t DR;      // 0x04
    volatile uint32_t BRR;     // 0x08
    volatile uint32_t CR1;     // 0x0C
    volatile uint32_t CR2;     // 0x10
    volatile uint32_t CR3;     // 0x14
    volatile uint32_t GTPR;    // 0x18

} USART_RegDef_t;

/* =========================
   PERIPHERAL POINTERS
   ========================= */

#define GPIOA  ((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define RCC    ((RCC_RegDef_t*)RCC_BASEADDR)
#define USART2 ((USART_RegDef_t*)USART2_BASEADDR)

/* =========================
   CLOCK ENABLE MACROS
   ========================= */

#define GPIOA_CLK_EN()   (RCC->AHB1ENR |= (1 << 0))
#define USART2_EN()      (RCC->APB1ENR |= (1 << 17))

#endif /* STM32F44XX_H_ */
