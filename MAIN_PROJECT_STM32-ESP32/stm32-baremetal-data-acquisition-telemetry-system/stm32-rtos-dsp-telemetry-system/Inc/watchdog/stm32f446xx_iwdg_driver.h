#ifndef INC_STM32F446XX_IWDG_DRIVER_H_
#define INC_STM32F446XX_IWDG_DRIVER_H_

#include "drivers/stm32f446xx.h"

/* IWDG Register Bit Definitions */
#define IWDG_KR_KEY_START     0xCCCC
#define IWDG_KR_KEY_REFRESH   0xAAAA
#define IWDG_KR_KEY_ACCESS    0x5555

/* Prescaler divider values */
#define IWDG_PR_DIV_4         0
#define IWDG_PR_DIV_8         1
#define IWDG_PR_DIV_16        2
#define IWDG_PR_DIV_32        3
#define IWDG_PR_DIV_64        4
#define IWDG_PR_DIV_128       5
#define IWDG_PR_DIV_256       6

/*
 * Configuration structure for IWDG
 */
typedef struct
{
    uint8_t  IWDG_Prescaler; /* Specifies the prescaler value (e.g., IWDG_PR_DIV_256) */
    uint16_t IWDG_Reload;    /* Specifies the reload value (max 0xFFF = 4095) */
} IWDG_Config_t;

/*
 * Handle structure for IWDG
 */
typedef struct
{
    IWDG_RegDef_t *pIWDG;    /* Pointer to the base address of IWDG peripheral */
    IWDG_Config_t IWDG_Config;
} IWDG_Handle_t;

/*
 * Function Prototypes
 */
void IWDG_Init(IWDG_Handle_t *pIWDGHandle);
void IWDG_Refresh(void);

#endif /* INC_STM32F446XX_IWDG_DRIVER_H_ */
