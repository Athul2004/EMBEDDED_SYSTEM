#ifndef STM32_F446XX_H_
#define STM32_F446XX_H_

/*
 * =========================================================================================
 *                                     1. TYPE DEFINITIONS
 * =========================================================================================
 * MISRA-C Rule: Use specific-length typedefs instead of basic C types.
 * Note: standard <stdint.h> is commented out due to toolchain issues in this environment.
 * We define our own types to ensure portability and consistency.
 */

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
typedef signed char        int8_t;
typedef signed short       int16_t;
typedef signed int         int32_t;
typedef signed long long   int64_t;
typedef float              float32_t;

/*
 * =========================================================================================
 *                                   2. HARDWARE BASE ADDRESSES
 * =========================================================================================
 */
#define RCC_BASE      0x40023800U
#define GPIOA_BASE    0x40020000U
#define GPIOB_BASE    0x40020400U
#define GPIOC_BASE    0x40020800U
#define ADC1_BASE     0x40012000U
#define DMA2_BASE     0x40026400U // DMA2 Base 
#define TIM2_BASE     0x40000000U
#define I2C1_BASE     0x40005400U
#define USART2_BASE   0x40004400U

/*
 * FPU Coprocessor Access Control Register
 * MISRA: Use volatile for register access
 */
#define FPU_CPACR     (*((volatile uint32_t*)0xE000ED88U))

/*
 * =========================================================================================
 *                                     3. PERIPHERAL REGISTER STRUCTURES
 * =========================================================================================
 * MISRA: structs used for memory mapping must be volatile to prevent compiler reordering/caching.
 */

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    volatile uint32_t RES0;
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t RES1[2];
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;
    volatile uint32_t RES2;
    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
    volatile uint32_t SSCGR;
    volatile uint32_t PLLI2SCFGR;
    volatile uint32_t PLLSAICFGR;
    volatile uint32_t DCKCFGR;
    volatile uint32_t CKGATENR;
    volatile uint32_t DCKCFGR2;
} RCC_TypeDef;

typedef struct {
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
} GPIO_TypeDef;

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t NDTR;
    volatile uint32_t PAR;
    volatile uint32_t M0AR;
    volatile uint32_t M1AR;
    volatile uint32_t FCR;
} DMA_Stream_TypeDef;

typedef struct {
    volatile uint32_t LISR;
    volatile uint32_t HISR;
    volatile uint32_t LIFCR;
    volatile uint32_t HIFCR;
} DMA_TypeDef;

typedef struct {
    volatile uint32_t SR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMPR1;
    volatile uint32_t SMPR2;
    volatile uint32_t JOFR1;
    volatile uint32_t JOFR2;
    volatile uint32_t JOFR3;
    volatile uint32_t JOFR4;
    volatile uint32_t HTR;
    volatile uint32_t LTR;
    volatile uint32_t SQR1;
    volatile uint32_t SQR2;
    volatile uint32_t SQR3;
    volatile uint32_t JSQR;
    volatile uint32_t JDR1;
    volatile uint32_t JDR2;
    volatile uint32_t JDR3;
    volatile uint32_t JDR4;
    volatile uint32_t DR;
} ADC_TypeDef;

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
} TIM_TypeDef;

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t OAR1;
    volatile uint32_t OAR2;
    volatile uint32_t DR;
    volatile uint32_t SR1;
    volatile uint32_t SR2;
    volatile uint32_t CCR;
    volatile uint32_t TRISE;
} I2C_TypeDef;

typedef struct {
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_TypeDef;

/*
 * =========================================================================================
 *                                     4. PERIPHERAL INSTANCE POINTERS
 * =========================================================================================
 */
#define RCC             ((RCC_TypeDef*)RCC_BASE)
#define GPIOA           ((GPIO_TypeDef*)GPIOA_BASE)
#define GPIOB           ((GPIO_TypeDef*)GPIOB_BASE)
#define GPIOC           ((GPIO_TypeDef*)GPIOC_BASE)
#define DMA2            ((DMA_TypeDef*)DMA2_BASE)
#define DMA2_Stream0    ((DMA_Stream_TypeDef*)(DMA2_BASE + 0x10U)) // Offset for Stream 0
#define ADC1            ((ADC_TypeDef*)ADC1_BASE)
#define TIM2            ((TIM_TypeDef*)TIM2_BASE)
#define I2C1            ((I2C_TypeDef*)I2C1_BASE)
#define USART1          ((USART_TypeDef*)0x40011000U) // Added missing USART1 Base (APB2)
#define USART2          ((USART_TypeDef*)USART2_BASE)
#define USART3          ((USART_TypeDef*)0x40004800U) // Added missing USART3 Base (APB1)
#define UART4           ((USART_TypeDef*)0x40004C00U) // Added missing UART4 Base (APB1)
#define UART5           ((USART_TypeDef*)0x40005000U) // Added missing UART5 Base (APB1)
#define USART6          ((USART_TypeDef*)0x40011400U) // Added missing USART6 Base (APB2)


/*
 * =========================================================================================
 *                                     5. COMMON BIT DEFINITIONS
 * =========================================================================================
 */
#define ENABLE          1U
#define DISABLE         0U
#define SET             1U
#define RESET           0U

// Peripheral Clock Enable Macros
#define ENABLE_GPIOA()  (RCC->AHB1ENR |= (1U << 0))
#define ENABLE_GPIOB()  (RCC->AHB1ENR |= (1U << 1))
#define ENABLE_GPIOC()  (RCC->AHB1ENR |= (1U << 2))
#define ENABLE_DMA2()   (RCC->AHB1ENR |= (1U << 22))
#define ENABLE_ADC1()   (RCC->APB2ENR |= (1U << 8))
#define ENABLE_TIM2()   (RCC->APB1ENR |= (1U << 0))
#define ENABLE_I2C1()   (RCC->APB1ENR |= (1U << 21))
#define ENABLE_UART2()  (RCC->APB1ENR |= (1U << 17))

#endif /* STM32_F446XX_H_ */
