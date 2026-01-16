/*
 * stm32_f446xx.h
 *
 *  Created on: 13-Jan-2026
 *      Author: KICHU
 */

#ifndef STM32_F446XX_H_
#define STM32_F446XX_H_

#include <stdint.h> // <--- FIX 1: Resolves 'unknown type name uint32_t'

//************************************UART*************************************************

// GPIO base addresses {AHB1 bus}
#define GPIOA_BASEADDR 0X40020000U
#define GPIOB_BASEADDR 0x40020400U
#define GPIOC_BASEADDR 0x40020800U
#define GPIOD_BASEADDR 0x40020C00U
#define GPIOE_BASEADDR 0x40021000U
#define GPIOF_BASEADDR 0x40021400U
#define GPIOG_BASEADDR 0x40021800U
#define GPIOH_BASEADDR 0x40021C00U

// GPIO Register structure
typedef struct
{
    volatile uint32_t MODER;    // 0x00
    volatile uint32_t OTYPER;   // 0x04
    volatile uint32_t OSPEEDR;  // 0x08
    volatile uint32_t PUPDR;    // 0x0C
    volatile uint32_t IDR;      // 0x10
    volatile uint32_t ODR;      // 0x14
    volatile uint32_t BSRR;     // 0x18
    volatile uint32_t LCKR;     // 0x1C
    volatile uint32_t AFRL;     // 0x20 <--- FIX 2: Restores AFRL member
    volatile uint32_t AFRH;     // 0x24 <--- FIX 2: Restores AFRH member
} GPIO_RegDef_t;

// GPIO pointers
#define GPIOA   ((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB   ((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC   ((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD   ((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE   ((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF   ((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG   ((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH   ((GPIO_RegDef_t*)GPIOH_BASEADDR)

// RCC Register structure
#define RCC_BASEADDR 0X40023800U

typedef struct
{
    volatile uint32_t CR;            // 0x00
    volatile uint32_t PLLCFGR;       // 0x04
    volatile uint32_t CFGR;          // 0x08
    volatile uint32_t CIR;           // 0x0C
    volatile uint32_t AHB1RSTR;      // 0x10
    volatile uint32_t AHB2RSTR;      // 0x14
    volatile uint32_t AHB3RSTR;      // 0x18
    volatile uint32_t RESERVED0;     // 0x1C
    volatile uint32_t APB1RSTR;      // 0x20
    volatile uint32_t APB2RSTR;      // 0x24
    volatile uint32_t RESERVED1[2];  // 0x28-0x2C
    volatile uint32_t AHB1ENR;       // 0x30
    volatile uint32_t AHB2ENR;       // 0x34
    volatile uint32_t AHB3ENR;       // 0x38
    volatile uint32_t RESERVED2;     // 0x3C
    volatile uint32_t APB1ENR;       // 0x40
    volatile uint32_t APB2ENR;       // 0x44
    // ... (rest truncated for brevity, but existing offsets are sufficient for now)
} RCC_RegDef_t;

#define RCC ((RCC_RegDef_t*)RCC_BASEADDR)

// Clock Enable Macros
#define GPIOA_CLK_EN() (RCC->AHB1ENR |= (1<<0)) // <--- FIX 3: Ensures this exists
#define GPIOB_CLK_EN() (RCC->AHB1ENR |= (1<<1))
#define GPIOC_CLK_EN() (RCC->AHB1ENR |= (1<<2))

// USART Base addresses
#define USART1_BASEADDR 0x40011000U
#define USART2_BASEADDR 0x40004400U
#define USART3_BASEADDR 0x40004800U
#define USART4_BASEADDR 0x40004C00U
#define USART5_BASEADDR 0x40005000U
#define USART6_BASEADDR 0x40011400U

// USART Register structure
typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_RegDef_t;

// USART pointers (FIXED POINTERS)
#define USART1 ((USART_RegDef_t*)USART1_BASEADDR)
#define USART2 ((USART_RegDef_t*)USART2_BASEADDR) // <--- FIX 4: Points to USART2 now
#define USART3 ((USART_RegDef_t*)USART3_BASEADDR)
#define USART4 ((USART_RegDef_t*)USART4_BASEADDR)
#define USART5 ((USART_RegDef_t*)USART5_BASEADDR)
#define USART6 ((USART_RegDef_t*)USART6_BASEADDR)

// USART Enable Macros
#define USART1_EN() (RCC->APB2ENR |= (1<<4))
#define USART2_EN() (RCC->APB1ENR |= (1<<17)) // <--- FIX 3: Ensures this exists
#define USART3_EN() (RCC->APB1ENR |= (1<<18))
#define USART6_EN() (RCC->APB2ENR |= (1<<5))  // Note: USART6 is typically bit 5

//****************************************ADC**************************************************

// ADC Base Address (APB2 Bus)
#define ADC1_BASEADDR 0x40012000U

// ADC Register Structure
typedef struct
{
    volatile uint32_t SR;     // Status Register
    volatile uint32_t CR1;    // Control Register 1
    volatile uint32_t CR2;    // Control Register 2
    volatile uint32_t SMPR1;  // Sample Time Register 1
    volatile uint32_t SMPR2;  // Sample Time Register 2
    volatile uint32_t JOFR1;  // Injected Channel Data Offset 1
    volatile uint32_t JOFR2;  // Injected Channel Data Offset 2
    volatile uint32_t JOFR3;  // Injected Channel Data Offset 3
    volatile uint32_t JOFR4;  // Injected Channel Data Offset 4
    volatile uint32_t HTR;    // Watchdog Higher Threshold
    volatile uint32_t LTR;    // Watchdog Lower Threshold
    volatile uint32_t SQR1;   // Regular Sequence Register 1
    volatile uint32_t SQR2;   // Regular Sequence Register 2
    volatile uint32_t SQR3;   // Regular Sequence Register 3
    volatile uint32_t JSQR;   // Injected Sequence Register
    volatile uint32_t JDR1;   // Injected Data Register 1
    volatile uint32_t JDR2;   // Injected Data Register 2
    volatile uint32_t JDR3;   // Injected Data Register 3
    volatile uint32_t JDR4;   // Injected Data Register 4
    volatile uint32_t DR;     // Regular Data Register
} ADC_RegDef_t;

// ADC Pointer
#define ADC1 ((ADC_RegDef_t*)ADC1_BASEADDR)

// ADC Clock Enable (Bit 8 of APB2ENR)
#define ADC1_EN() (RCC->APB2ENR |= (1 << 8))

#endif /* STM32_F446XX_H_ */
