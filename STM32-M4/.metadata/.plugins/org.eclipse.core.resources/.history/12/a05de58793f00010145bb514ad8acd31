/*
 * stm32_f446xx.h
 *
 *  Created on: Nov 19, 2025
 *      Author: athul
 */

#ifndef STM32_F446XX_H_
#define STM32_F446XX_H_

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
    volatile uint32_t MODER;    // GPIO port mode register              (0x00)
    volatile uint32_t OTYPER;   // GPIO port output type register       (0x04)
    volatile uint32_t OSPEEDR;  // GPIO port output speed register      (0x08)
    volatile uint32_t PUPDR;    // GPIO port pull-up/pull-down register (0x0C)
    volatile uint32_t IDR;      // GPIO port input data register        (0x10)
    volatile uint32_t ODR;      // GPIO port output data register       (0x14)
    volatile uint32_t BSRR;     // GPIO port bit set/reset register     (0x18)
    volatile uint32_t LCKR;     // GPIO port configuration lock register(0x1C)
    volatile uint32_t AFRL;     // GPIO alternate function low register (0x20)
    volatile uint32_t AFRH;     // GPIO alternate function high register(0x24)
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

//-----------------------------------------------------------------------

// 						RCC Register base address and register structure

// RCC BASE ADDRESS
#define RCC_BASEADDR 0X40023800U

// RCC register structure

typedef struct
{
    volatile uint32_t CR;            // RCC clock control register                    (0x00)
    volatile uint32_t PLLCFGR;       // RCC PLL configuration register                (0x04)
    volatile uint32_t CFGR;          // RCC clock configuration register              (0x08)
    volatile uint32_t CIR;           // RCC clock interrupt register                  (0x0C)
    volatile uint32_t AHB1RSTR;      // RCC AHB1 peripheral reset register            (0x10)
    volatile uint32_t AHB2RSTR;      // RCC AHB2 peripheral reset register            (0x14)
    volatile uint32_t AHB3RSTR;      // RCC AHB3 peripheral reset register            (0x18)
    volatile uint32_t RESERVED0;     // Reserved                                      (0x1C)
    volatile uint32_t APB1RSTR;      // RCC APB1 peripheral reset register            (0x20)
    volatile uint32_t APB2RSTR;      // RCC APB2 peripheral reset register            (0x24)
    volatile uint32_t RESERVED1[2];  // Reserved                                      (0x28-0x2C)
    volatile uint32_t AHB1ENR;       // RCC AHB1 peripheral clock enable register     (0x30)
    volatile uint32_t AHB2ENR;       // RCC AHB2 peripheral clock enable register     (0x34)
    volatile uint32_t AHB3ENR;       // RCC AHB3 peripheral clock enable register     (0x38)
    volatile uint32_t RESERVED2;     // Reserved                                      (0x3C)
    volatile uint32_t APB1ENR;       // RCC APB1 peripheral clock enable register     (0x40)
    volatile uint32_t APB2ENR;       // RCC APB2 peripheral clock enable register     (0x44)
    volatile uint32_t RESERVED3[2];  // Reserved                                      (0x48-0x4C)
    volatile uint32_t AHB1LPENR;     // RCC AHB1 peripheral clock enable in low power (0x50)
    volatile uint32_t AHB2LPENR;     // RCC AHB2 peripheral clock enable in low power (0x54)
    volatile uint32_t AHB3LPENR;     // RCC AHB3 peripheral clock enable in low power (0x58)
    volatile uint32_t RESERVED4;     // Reserved                                      (0x5C)
    volatile uint32_t APB1LPENR;     // RCC APB1 peripheral clock enable in low power (0x60)
    volatile uint32_t APB2LPENR;     // RCC APB2 peripheral clock enable in low power (0x64)
    volatile uint32_t RESERVED5[2];  // Reserved                                      (0x68-0x6C)
    volatile uint32_t BDCR;          // RCC Backup domain control register            (0x70)
    volatile uint32_t CSR;           // RCC clock control & status register           (0x74)
    volatile uint32_t RESERVED6[2];  // Reserved                                      (0x78-0x7C)
    volatile uint32_t SSCGR;         // RCC spread spectrum clock generation register (0x80)
    volatile uint32_t PLLI2SCFGR;    // RCC PLLI2S configuration register             (0x84)
    volatile uint32_t PLLSAICFGR;    // RCC PLLSAI configuration register             (0x88)
    volatile uint32_t DCKCFGR;       // RCC Dedicated Clocks configuration register   (0x8C)
    volatile uint32_t CKGATENR;      // RCC clocks gated enable register              (0x90)
    volatile uint32_t DCKCFGR2;      // RCC Dedicated Clocks configuration register 2 (0x94)
} RCC_RegDef_t;

// RCC pointer
#define RCC ((RCC_RegDef_t*)RCC_BASEADDR)

// clock enable for GPIO
#define GPIOA_CLK_EN() (RCC->AHB1ENR)|=(1<<0)
#define GPIOB_CLK_EN() (RCC->AHB1ENR)|=(1<<1)
#define GPIOC_CLK_EN() (RCC->AHB1ENR)|=(1<<2)
#define GPIOD_CLK_EN() (RCC->AHB1ENR)|=(1<<3)
#define GPIOE_CLK_EN() (RCC->AHB1ENR)|=(1<<4)
#define GPIOF_CLK_EN() (RCC->AHB1ENR)|=(1<<5)
#define GPIOG_CLK_EN() (RCC->AHB1ENR)|=(1<<6)
#define GPIOH_CLK_EN() (RCC->AHB1ENR)|=(1<<7)

//-----------------------------------------------------------

// 						I2C Protocol

// I2C base addresses
#define I2C1_BASEADDR 0x40005400U
#define I2C2_BASEADDR 0x40005800U
#define I2C3_BASEADDR 0x40005C00U

// I2C Register Structure
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

}I2C_RegDef_t;

// I2C Pointers
#define I2C1 ((I2C_RegDef_t*)I2C1_BASEADDR)
#define I2C2 ((I2C_RegDef_t*)I2C2_BASEADDR)
#define I2C3 ((I2C_RegDef_t*)I2C3_BASEADDR)


// I2C enable

#define I2C1_EN() (RCC->APB1ENR|=(1<<21))
#define I2C2_EN() (RCC->APB1ENR|=(1<<22))
#define I2C3_EN() (RCC->APB1ENR|=(1<<23))


//-----------------------------------------------------

// 						SPI Protocol

// SPI base addresses
#define SPI1_BASEADDR 0x40013000U		// ON BUS APB2
#define SPI2_BASEADDR 0x40003800U		// ON BUS APB1
#define SPI3_BASEADDR 0x40003C00U 		// ON BUS APB1
#define SPI4_BASEADDR 0x40013400U 		// ON BUS APB2

// SPI Register structure

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

// SPI pointers
#define SPI1 ((SPI_RegDef_t*)SPI1_BASEADDR)
#define SPI2 ((SPI_RegDef_t*)SPI2_BASEADDR)
#define SPI3 ((SPI_RegDef_t*)SPI3_BASEADDR)
#define SPI4 ((SPI_RegDef_t*)SPI4_BASEADDR)

// SPI Enable
#define SPI1_EN() (RCC->APB2ENR|=(1<<12))
#define SPI2_EN() (RCC->APB1ENR|=(1<<14))
#define SPI3_EN() (RCC->APB1ENR|=(1<<15))
#define SPI4_EN() (RCC->APB2ENR|=(1<<13))

//-------------------------------------------------------------

// 						USART Protocol
// USART Base addresses
#define USART1_BASEADDR 0x40011000U		// APB2
#define USART2_BASEADDR 0x40004400U		// APB1
#define USART3_BASEADDR 0x40004800U 	// APB1
#define USART4_BASEADDR 0x40004C00U		// APB1
#define USART5_BASEADDR 0x40005000U		// APB1
#define USART6_BASEADDR 0x40011400U		// APB2

// USART register structure
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


// USART pointers
#define USART1 ((USART_RegDef_t*)USART1_BASEADDR)
#define USART2 ((USART_RegDef_t*)USART1_BASEADDR)
#define USART3 ((USART_RegDef_t*)USART1_BASEADDR)
#define USART4 ((USART_RegDef_t*)USART1_BASEADDR)
#define USART5 ((USART_RegDef_t*)USART1_BASEADDR)
#define USART6 ((USART_RegDef_t*)USART1_BASEADDR)


// USART Enable
#define USART1_EN() (RCC->APB2ENR|=(1<<4))
#define USART2_EN() (RCC->APB1ENR|=(1<<17))
#define USART3_EN() (RCC->APB1ENR|=(1<<18))
#define USART4_EN() (RCC->APB1ENR|=(1<<19))
#define USART5_EN() (RCC->APB1ENR|=(1<<20))
#define USART6_EN() (RCC->APB2ENR|=(1<<4))

//---------------------------------------------------------------------------

// 						CAN Protocol

// CAN base addresses
#define CAN1_BASEADDR 0x40006400U	// APB1
#define CAN2_BASEADDR 0x40006800U	// APB1

// CAN register structure
typedef struct
{
	volatile uint32_t MCR;
	volatile uint32_t MSR;
	volatile uint32_t TSR;
	volatile uint32_t RF0R;
	volatile uint32_t RF1R;
	volatile uint32_t IER;
	volatile uint32_t ESR;
	volatile uint32_t BTR;
	volatile uint32_t TI0R;
	volatile uint32_t TDT0R;
	volatile uint32_t TDL0R;
	volatile uint32_t TDH0R;
	volatile uint32_t TI1R;
	volatile uint32_t TDT1R;
	volatile uint32_t TDL1R;
	volatile uint32_t TDH1R;
	volatile uint32_t TI2R;
	volatile uint32_t TDT2R;
	volatile uint32_t TDL2R;
	volatile uint32_t TDH2R;
	volatile uint32_t RI0R;
	volatile uint32_t RDT0R;
	volatile uint32_t RDL0R;
	volatile uint32_t RDH0R;
	volatile uint32_t RI1R;
	volatile uint32_t RDT1R;
	volatile uint32_t RDL1R;
	volatile uint32_t RDH1R;
	volatile uint32_t RESERVED1;
	volatile uint32_t RESERVED2;
	volatile uint32_t FS1R;
	volatile uint32_t RESERVED3;
	volatile uint32_t FFA1R;
	volatile uint32_t FA1R;
	volatile uint32_t F0R1;
	volatile uint32_t F0R2;
	volatile uint32_t F1R1;
	volatile uint32_t F1R2;
	volatile uint32_t F27R1;
	volatile uint32_t F27R2;
}CAN_RegDef_t;

// CAN Pointer
#define CAN1 ((CAN_RegDef_t*)CAN1_BASEADDR)
#define CAN2 ((CAN_RegDef_t*)CAN2_BASEADDR)

// CAN Enable

#define CAN1_EN() (RCC->APB1ENR|=(1<<25))
#define CAN2_EN() (RCC->APB1ENR|=(1<<26))





#endif /* STM32_F446XX_H_ */
