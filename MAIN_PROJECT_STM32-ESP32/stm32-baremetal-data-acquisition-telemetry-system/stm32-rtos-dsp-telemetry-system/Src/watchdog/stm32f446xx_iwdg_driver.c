#include "watchdog/stm32f446xx_iwdg_driver.h"

/*
 * Initialize the IWDG with the given configuration
 */
void IWDG_Init(IWDG_Handle_t *pIWDGHandle)
{
    // 1. Enable the IWDG (this forces the LSI oscillator ON)
    pIWDGHandle->pIWDG->KR = IWDG_KR_KEY_START;

    // 2. Enable write access to PR, RLR registers
    pIWDGHandle->pIWDG->KR = IWDG_KR_KEY_ACCESS;
    
    // 3. Write prescaler value
    pIWDGHandle->pIWDG->PR = pIWDGHandle->IWDG_Config.IWDG_Prescaler;
    
    // 4. Write reload value
    pIWDGHandle->pIWDG->RLR = pIWDGHandle->IWDG_Config.IWDG_Reload;
    
    // 5. Wait for registers to be updated (wait for SR bits to clear)
    // SR bit 0: RVU (Reload Value Update), bit 1: PVU (Prescaler Value Update)
    while(pIWDGHandle->pIWDG->SR != 0);
    
    // 6. Refresh the counter
    IWDG_Refresh();
}

/*
 * Refresh (feed) the watchdog counter
 */
void IWDG_Refresh(void)
{
    IWDG->KR = IWDG_KR_KEY_REFRESH;
}
