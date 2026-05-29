/*
 * stm32_f446xx.h
 *
 *  Created on: 30-May-2026
 *      Author: athul
 */

#ifndef STM32_F446XX_H_
#define STM32_F446XX_H_

#include <stdint.h>

/* ========================= */
/*       BASE ADDRESSES      */
/* ========================= */

#define PERIPH_BASE         0x40000000UL

#define AHB1PERIPH_BASE     0x40020000UL
#define APB1PERIPH_BASE     PERIPH_BASE

#define GPIOA_BASE          (AHB1PERIPH_BASE + 0x0000)
#define GPIOB_BASE          (AHB1PERIPH_BASE + 0x0400)

#define RCC_BASE            (AHB1PERIPH_BASE + 0x3800)

#define I2C1_BASE           (APB1PERIPH_BASE + 0x5400)

#define USART2_BASE         (APB1PERIPH_BASE + 0x4400)

/* ========================= */
/*       GPIO STRUCTURE      */
/* ========================= */

typedef struct
{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];

}GPIO_TypeDef;

/* ========================= */
/*        RCC STRUCTURE      */
/* ========================= */

typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;

    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    uint32_t RESERVED0;

    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;

    uint32_t RESERVED1[2];

    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;

    uint32_t RESERVED2;

    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;

}RCC_TypeDef;

/* ========================= */
/*       I2C STRUCTURE       */
/* ========================= */

typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t OAR1;
    volatile uint32_t OAR2;
    volatile uint32_t DR;
    volatile uint32_t SR1;
    volatile uint32_t SR2;
    volatile uint32_t CCR;
    volatile uint32_t TRISE;
    volatile uint32_t FLTR;

}I2C_TypeDef;

/* ========================= */
/*      USART STRUCTURE      */
/* ========================= */

typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;

}USART_TypeDef;

/* ========================= */
/*     PERIPHERAL DEFINES    */
/* ========================= */

#define GPIOA      ((GPIO_TypeDef*)GPIOA_BASE)
#define GPIOB      ((GPIO_TypeDef*)GPIOB_BASE)

#define RCC        ((RCC_TypeDef*)RCC_BASE)

#define I2C1       ((I2C_TypeDef*)I2C1_BASE)

#define USART2     ((USART_TypeDef*)USART2_BASE)

/* ========================= */
/*      CLOCK MACROS         */
/* ========================= */

#define GPIOA_CLK_EN()      (RCC->AHB1ENR |= (1<<0))
#define GPIOB_CLK_EN()      (RCC->AHB1ENR |= (1<<1))

#define USART2_CLK_EN()     (RCC->APB1ENR |= (1<<17))

#define I2C1_CLK_EN()       (RCC->APB1ENR |= (1<<21))

#endif
