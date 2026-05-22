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

typedef unsigned char      uint8_t;     // Define 8-bit unsigned integer
typedef unsigned short     uint16_t;    // Define 16-bit unsigned integer
typedef unsigned int       uint32_t;    // Define 32-bit unsigned integer
typedef unsigned long long uint64_t;    // Define 64-bit unsigned integer
typedef signed char        int8_t;      // Define 8-bit signed integer
typedef signed short       int16_t;     // Define 16-bit signed integer
typedef signed int         int32_t;     // Define 32-bit signed integer
typedef signed long long   int64_t;     // Define 64-bit signed integer
typedef float              float32_t;   // Define 32-bit floating point number

/*
 * =========================================================================================
 *                                   2. HARDWARE BASE ADDRESSES
 * =========================================================================================
 */
#define RCC_BASE      0x40023800U       // Base address for Reset and Clock Control (RCC)
#define GPIOA_BASE    0x40020000U       // Base address for GPIO Port A
#define GPIOB_BASE    0x40020400U       // Base address for GPIO Port B
#define GPIOC_BASE    0x40020800U       // Base address for GPIO Port C
#define ADC1_BASE     0x40012000U       // Base address for ADC1 peripheral
#define DMA2_BASE     0x40026400U       // Base address for DMA2 controller
#define TIM2_BASE     0x40000000U       // Base address for Timer 2
#define I2C1_BASE     0x40005400U       // Base address for I2C1 peripheral
#define USART2_BASE   0x40004400U       // Base address for USART2 peripheral

/*
 * FPU Coprocessor Access Control Register
 * MISRA: Use volatile for register access
 */
#define FPU_CPACR     (*((volatile uint32_t*)0xE000ED88U)) // Pointer to FPU Access Control Register

/*
 * =========================================================================================
 *                                     3. PERIPHERAL REGISTER STRUCTURES
 * =========================================================================================
 * MISRA: structs used for memory mapping must be volatile to prevent compiler reordering/caching.
 */

// Structure definition for RCC registers
typedef struct {
    volatile uint32_t CR;           // Clock Control Register
    volatile uint32_t PLLCFGR;      // PLL Configuration Register
    volatile uint32_t CFGR;         // Clock Configuration Register
    volatile uint32_t CIR;          // Clock Interrupt Register
    volatile uint32_t AHB1RSTR;     // AHB1 Peripheral Reset Register
    volatile uint32_t AHB2RSTR;     // AHB2 Peripheral Reset Register
    volatile uint32_t AHB3RSTR;     // AHB3 Peripheral Reset Register
    volatile uint32_t RES0;         // Reserved
    volatile uint32_t APB1RSTR;     // APB1 Peripheral Reset Register
    volatile uint32_t APB2RSTR;     // APB2 Peripheral Reset Register
    volatile uint32_t RES1[2];      // Reserved
    volatile uint32_t AHB1ENR;      // AHB1 Peripheral Clock Enable Register
    volatile uint32_t AHB2ENR;      // AHB2 Peripheral Clock Enable Register
    volatile uint32_t AHB3ENR;      // AHB3 Peripheral Clock Enable Register
    volatile uint32_t RES2;         // Reserved
    volatile uint32_t APB1ENR;      // APB1 Peripheral Clock Enable Register
    volatile uint32_t APB2ENR;      // APB2 Peripheral Clock Enable Register
    volatile uint32_t BDCR;         // Backup Domain Control Register
    volatile uint32_t CSR;          // Clock Control & Status Register
    volatile uint32_t SSCGR;        // Spread Spectrum Clock Generation Register
    volatile uint32_t PLLI2SCFGR;   // PLLI2S Configuration Register
    volatile uint32_t PLLSAICFGR;   // PLLSAI Configuration Register
    volatile uint32_t DCKCFGR;      // Dedicated Clock Configuration Register
    volatile uint32_t CKGATENR;     // Clock Gating Control Register
    volatile uint32_t DCKCFGR2;     // Dedicated Clock Configuration Register 2
} RCC_TypeDef;

// Structure definition for GPIO registers
typedef struct {
    volatile uint32_t MODER;        // GPIO Mode Register
    volatile uint32_t OTYPER;       // GPIO Output Type Register
    volatile uint32_t OSPEEDR;      // GPIO Output Speed Register
    volatile uint32_t PUPDR;        // GPIO Pull-up/Pull-down Register
    volatile uint32_t IDR;          // GPIO Input Data Register
    volatile uint32_t ODR;          // GPIO Output Data Register
    volatile uint32_t BSRR;         // GPIO Bit Set/Reset Register
    volatile uint32_t LCKR;         // GPIO Configuration Lock Register
    volatile uint32_t AFRL;         // GPIO Alternate Function Low Register
    volatile uint32_t AFRH;         // GPIO Alternate Function High Register
} GPIO_TypeDef;

// Structure definition for DMA Stream registers
typedef struct {
    volatile uint32_t CR;           // DMA Stream x Configuration Register
    volatile uint32_t NDTR;         // DMA Stream x Number of Data Register
    volatile uint32_t PAR;          // DMA Stream x Peripheral Address Register
    volatile uint32_t M0AR;         // DMA Stream x Memory 0 Address Register
    volatile uint32_t M1AR;         // DMA Stream x Memory 1 Address Register
    volatile uint32_t FCR;          // DMA Stream x FIFO Control Register
} DMA_Stream_TypeDef;

// Structure definition for DMA Controller registers
typedef struct {
    volatile uint32_t LISR;         // DMA Low Interrupt Status Register
    volatile uint32_t HISR;         // DMA High Interrupt Status Register
    volatile uint32_t LIFCR;        // DMA Low Interrupt Flag Clear Register
    volatile uint32_t HIFCR;        // DMA High Interrupt Flag Clear Register
} DMA_TypeDef;

// Structure definition for ADC registers
typedef struct {
    volatile uint32_t SR;           // ADC Status Register
    volatile uint32_t CR1;          // ADC Control Register 1
    volatile uint32_t CR2;          // ADC Control Register 2
    volatile uint32_t SMPR1;        // ADC Sample Time Register 1
    volatile uint32_t SMPR2;        // ADC Sample Time Register 2
    volatile uint32_t JOFR1;        // ADC Injected Channel Data Offset Register 1
    volatile uint32_t JOFR2;        // ADC Injected Channel Data Offset Register 2
    volatile uint32_t JOFR3;        // ADC Injected Channel Data Offset Register 3
    volatile uint32_t JOFR4;        // ADC Injected Channel Data Offset Register 4
    volatile uint32_t HTR;          // ADC Watchdog Higher Threshold Register
    volatile uint32_t LTR;          // ADC Watchdog Lower Threshold Register
    volatile uint32_t SQR1;         // ADC Regular Sequence Register 1
    volatile uint32_t SQR2;         // ADC Regular Sequence Register 2
    volatile uint32_t SQR3;         // ADC Regular Sequence Register 3
    volatile uint32_t JSQR;         // ADC Injected Sequence Register
    volatile uint32_t JDR1;         // ADC Injected Data Register 1
    volatile uint32_t JDR2;         // ADC Injected Data Register 2
    volatile uint32_t JDR3;         // ADC Injected Data Register 3
    volatile uint32_t JDR4;         // ADC Injected Data Register 4
    volatile uint32_t DR;           // ADC Regular Data Register
} ADC_TypeDef;

// Structure definition for Timer registers
typedef struct {
    volatile uint32_t CR1;          // TIM control register 1
    volatile uint32_t CR2;          // TIM control register 2
    volatile uint32_t SMCR;         // TIM slave mode control register
    volatile uint32_t DIER;         // TIM DMA/interrupt enable register
    volatile uint32_t SR;           // TIM status register
    volatile uint32_t EGR;          // TIM event generation register
    volatile uint32_t CCMR1;        // TIM capture/compare mode register 1
    volatile uint32_t CCMR2;        // TIM capture/compare mode register 2
    volatile uint32_t CCER;         // TIM capture/compare enable register
    volatile uint32_t CNT;          // TIM counter
    volatile uint32_t PSC;          // TIM prescaler
    volatile uint32_t ARR;          // TIM auto-reload register
} TIM_TypeDef;

// Structure definition for I2C registers
typedef struct {
    volatile uint32_t CR1;          // I2C Control register 1
    volatile uint32_t CR2;          // I2C Control register 2
    volatile uint32_t OAR1;         // I2C Own address register 1
    volatile uint32_t OAR2;         // I2C Own address register 2
    volatile uint32_t DR;           // I2C Data register
    volatile uint32_t SR1;          // I2C Status register 1
    volatile uint32_t SR2;          // I2C Status register 2
    volatile uint32_t CCR;          // I2C Clock control register
    volatile uint32_t TRISE;        // I2C TRISE register
} I2C_TypeDef;

// Structure definition for USART registers
typedef struct {
    volatile uint32_t SR;           // USART Status register
    volatile uint32_t DR;           // USART Data register
    volatile uint32_t BRR;          // USART Baud rate register
    volatile uint32_t CR1;          // USART Control register 1
    volatile uint32_t CR2;          // USART Control register 2
    volatile uint32_t CR3;          // USART Control register 3
    volatile uint32_t GTPR;         // USART Guard time and prescaler register
} USART_TypeDef;

/*
 * =========================================================================================
 *                                     4. PERIPHERAL INSTANCE POINTERS
 * =========================================================================================
 */
#define RCC             ((RCC_TypeDef*)RCC_BASE)            // Pointer to RCC register struct
#define GPIOA           ((GPIO_TypeDef*)GPIOA_BASE)         // Pointer to GPIOA register struct
#define GPIOB           ((GPIO_TypeDef*)GPIOB_BASE)         // Pointer to GPIOB register struct
#define GPIOC           ((GPIO_TypeDef*)GPIOC_BASE)         // Pointer to GPIOC register struct
#define DMA2            ((DMA_TypeDef*)DMA2_BASE)           // Pointer to DMA2 register struct
#define DMA2_Stream0    ((DMA_Stream_TypeDef*)(DMA2_BASE + 0x10U)) // Pointer to DMA2 Stream 0 (Offset 0x10)
#define ADC1            ((ADC_TypeDef*)ADC1_BASE)           // Pointer to ADC1 register struct
#define TIM2            ((TIM_TypeDef*)TIM2_BASE)           // Pointer to TIM2 register struct
#define I2C1            ((I2C_TypeDef*)I2C1_BASE)           // Pointer to I2C1 register struct
#define USART1          ((USART_TypeDef*)0x40011000U)       // Pointer to USART1 register struct (APB2)
#define USART2          ((USART_TypeDef*)USART2_BASE)       // Pointer to USART2 register struct (APB1)
#define USART3          ((USART_TypeDef*)0x40004800U)       // Pointer to USART3 register struct (APB1)
#define UART4           ((USART_TypeDef*)0x40004C00U)       // Pointer to UART4 register struct (APB1)
#define UART5           ((USART_TypeDef*)0x40005000U)       // Pointer to UART5 register struct (APB1)
#define USART6          ((USART_TypeDef*)0x40011400U)       // Pointer to USART6 register struct (APB2)


/*
 * =========================================================================================
 *                                     5. COMMON BIT DEFINITIONS
 * =========================================================================================
 */
#define ENABLE          1U      // Logical high state / Enable
#define DISABLE         0U      // Logical low state / Disable
#define SET             1U      // Set bit
#define RESET           0U      // Reset bit

// Peripheral Clock Enable Macros
#define ENABLE_GPIOA()  (RCC->AHB1ENR |= (1U << 0))    // Enable clock for GPIOA (Bit 0)
#define ENABLE_GPIOB()  (RCC->AHB1ENR |= (1U << 1))    // Enable clock for GPIOB (Bit 1)
#define ENABLE_GPIOC()  (RCC->AHB1ENR |= (1U << 2))    // Enable clock for GPIOC (Bit 2)
#define ENABLE_DMA2()   (RCC->AHB1ENR |= (1U << 22))   // Enable clock for DMA2 (Bit 22)
#define ENABLE_ADC1()   (RCC->APB2ENR |= (1U << 8))    // Enable clock for ADC1 (Bit 8)
#define ENABLE_TIM2()   (RCC->APB1ENR |= (1U << 0))    // Enable clock for TIM2 (Bit 0)
#define ENABLE_I2C1()   (RCC->APB1ENR |= (1U << 21))   // Enable clock for I2C1 (Bit 21)
#define ENABLE_UART2()  (RCC->APB1ENR |= (1U << 17))   // Enable clock for USART2 (Bit 17)

#endif /* STM32_F446XX_H_ */
