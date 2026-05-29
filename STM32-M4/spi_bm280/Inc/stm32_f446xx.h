/*
 * stm32_f446xx.h
 *
 *  Created on: 29-May-2026
 *      Author: athul
 */

#ifndef STM32_F446XX_H_
#define STM32_F446XX_H_

#include <stdint.h>

/*=========================================================
                GPIO BASE ADDRESSES
=========================================================*/

#define GPIOA_BASEADDR      0x40020000U
#define GPIOB_BASEADDR      0x40020400U
#define GPIOC_BASEADDR      0x40020800U
#define GPIOD_BASEADDR      0x40020C00U
#define GPIOE_BASEADDR      0x40021000U
#define GPIOF_BASEADDR      0x40021400U
#define GPIOG_BASEADDR      0x40021800U
#define GPIOH_BASEADDR      0x40021C00U

/*=========================================================
                RCC BASE ADDRESS
=========================================================*/

#define RCC_BASEADDR        0x40023800U

/*=========================================================
                SPI BASE ADDRESSES
=========================================================*/

#define SPI1_BASEADDR       0x40013000U
#define SPI2_BASEADDR       0x40003800U
#define SPI3_BASEADDR       0x40003C00U

/*=========================================================
                USART BASE ADDRESSES
=========================================================*/

#define USART1_BASEADDR     0x40011000U
#define USART2_BASEADDR     0x40004400U
#define USART3_BASEADDR     0x40004800U

/*=========================================================
                GPIO REGISTERS
=========================================================*/

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
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;

}GPIO_RegDef_t;

/*=========================================================
                RCC REGISTERS
=========================================================*/

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

}RCC_RegDef_t;

/*=========================================================
                SPI REGISTERS
=========================================================*/

typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t CRCPR;
    volatile uint32_t RXCRCR;
    volatile uint32_t TXCRCR;
    volatile uint32_t I2SCFGR;
    volatile uint32_t I2SPR;

}SPI_RegDef_t;

/*=========================================================
                USART REGISTERS
=========================================================*/

typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;

}USART_RegDef_t;

/*=========================================================
                PERIPHERAL POINTERS
=========================================================*/

#define GPIOA ((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB ((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC ((GPIO_RegDef_t*)GPIOC_BASEADDR)

#define RCC ((RCC_RegDef_t*)RCC_BASEADDR)

#define SPI1 ((SPI_RegDef_t*)SPI1_BASEADDR)
#define SPI2 ((SPI_RegDef_t*)SPI2_BASEADDR)
#define SPI3 ((SPI_RegDef_t*)SPI3_BASEADDR)

#define USART1 ((USART_RegDef_t*)USART1_BASEADDR)
#define USART2 ((USART_RegDef_t*)USART2_BASEADDR)
#define USART3 ((USART_RegDef_t*)USART3_BASEADDR)

/*=========================================================
                CLOCK ENABLE MACROS
=========================================================*/

#define GPIOA_CLK_EN()  (RCC->AHB1ENR |= (1<<0))
#define GPIOB_CLK_EN()  (RCC->AHB1ENR |= (1<<1))
#define GPIOC_CLK_EN()  (RCC->AHB1ENR |= (1<<2))

#define SPI1_CLK_EN()   (RCC->APB2ENR |= (1<<12))
#define SPI2_CLK_EN()   (RCC->APB1ENR |= (1<<14))
#define SPI3_CLK_EN()   (RCC->APB1ENR |= (1<<15))

#define USART1_CLK_EN() (RCC->APB2ENR |= (1<<4))
#define USART2_CLK_EN() (RCC->APB1ENR |= (1<<17))
#define USART3_CLK_EN() (RCC->APB1ENR |= (1<<18))

/*=========================================================
                GPIO MODES
=========================================================*/

#define GPIO_MODE_INPUT     0
#define GPIO_MODE_OUTPUT    1
#define GPIO_MODE_AF        2
#define GPIO_MODE_ANALOG    3

/*=========================================================
                SPI BITS
=========================================================*/

#define SPI_CR1_CPHA        0
#define SPI_CR1_CPOL        1
#define SPI_CR1_MSTR        2
#define SPI_CR1_BR          3
#define SPI_CR1_SPE         6
#define SPI_CR1_LSBFIRST    7
#define SPI_CR1_SSI         8
#define SPI_CR1_SSM         9

#define SPI_SR_RXNE         0
#define SPI_SR_TXE          1
#define SPI_SR_BSY          7

/*=========================================================
                USART BITS
=========================================================*/

#define USART_CR1_RE        2
#define USART_CR1_TE        3
#define USART_CR1_UE        13

#define USART_SR_RXNE       5
#define USART_SR_TXE        7

#endif
