#include <stdio.h>
#include "drivers/stm32f446xx.h"
#include "drivers/delay.h"
#include "drivers/stm32f446xx_gpio_driver.h"
#include "drivers/stm32f446xx_i2c_driver.h"
#include "drivers/stm32f446xx_usart_driver.h"
#include "sensors/bmp280.h"
#include "dsp/dsp_engine.h"
#include "esp32-telemetry/spi_telemetry.h"
#include "gsm/gsm.h"
#include "rtos/kernel.h"
#include "app/app_tasks.h"
#include "watchdog/stm32f446xx_iwdg_driver.h"
#include "BoardConfig.h"

I2C_Handle_t I2C1Handle;
USART_Handle_t USART2Handle;

void I2C1_GPIOInits(void) {
    // CRITICAL BUG FIX: The GPIOB clock MUST be enabled before configuring PB8 and PB9!
    // Without this, the I2C pins remain disconnected internally and I2C hangs instantly!
    GPIO_PeriClockControl(GPIOB, ENABLE);

    GPIO_Handle_t I2CPins;
    I2CPins.pGPIOx = BOARD_I2C1_PORT;
    I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = BOARD_I2C1_AF;
    I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
    I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    
    // SCL
    I2CPins.GPIO_PinConfig.GPIO_PinNumber = BOARD_I2C1_SCL_PIN;
    GPIO_Init(&I2CPins);

    // SDA
    I2CPins.GPIO_PinConfig.GPIO_PinNumber = BOARD_I2C1_SDA_PIN;
    GPIO_Init(&I2CPins);
}

void I2C1_Inits(void) {
    I2C1Handle.pI2Cx = I2C1;
    I2C1Handle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;
    I2C1Handle.I2C_Config.I2C_DeviceAddress = 0x61;
    I2C1Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
    I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

    I2C_Init(&I2C1Handle);
    I2C_PeripheralControl(I2C1, ENABLE);
}

void USART2_GPIOInits(void) {
    GPIO_Handle_t USARTPins;
    USARTPins.pGPIOx = BOARD_USART2_PORT;
    USARTPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    USARTPins.GPIO_PinConfig.GPIO_PinAltFunMode = BOARD_USART2_AF;
    USARTPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    USARTPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    USARTPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    // TX
    USARTPins.GPIO_PinConfig.GPIO_PinNumber = BOARD_USART2_TX_PIN;
    GPIO_Init(&USARTPins);

    // RX
    USARTPins.GPIO_PinConfig.GPIO_PinNumber = BOARD_USART2_RX_PIN;
    GPIO_Init(&USARTPins);
}

void USART2_Inits(void) {
    USART2Handle.pUSARTx = USART2;
    USART2Handle.USART_Config.USART_Baud = USART_STD_BAUD_115200;
    USART2Handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
    USART2Handle.USART_Config.USART_Mode = USART_MODE_TXRX;
    USART2Handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
    USART2Handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
    USART2Handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;

    USART_Init(&USART2Handle);
    USART_PeripheralControl(USART2, ENABLE);
}

#ifdef __GNUC__
int __io_putchar(int ch) {
    uint8_t c = ch;
    USART_SendData(&USART2Handle, &c, 1);
    return ch;
}

int _write(int file, char *ptr, int len) {
    USART_SendData(&USART2Handle, (uint8_t*)ptr, len);
    return len;
}
#endif

// RTOS Application Tasks and Variables moved to app/app_tasks.c

int main(void) {
    // 0. Enable FPU (Coprocessors CP10 and CP11)
    SCB_CPACR |= ((3UL << 10*2) | (3UL << 11*2));

    // 1. Initialize SysTick for delays
    SysTick_Init();

    // 2. Initialize UART for debug output
    USART2_GPIOInits();
    USART2_Inits();
    printf("\r\n--- STM32F446RE Advanced DSP Altimeter ---\r\n");

    // 3. Initialize I2C1
    I2C1_GPIOInits();
    I2C1_Inits();
    printf("I2C1 Initialized.\r\n");

    // 4. Initialize BMP280
    if (BMP280_Init()) {
        printf("BMP280 Initialized successfully.\r\n");
    } else {
        printf("Failed to initialize BMP280. Check connections.\r\n");
        while (1) delay_ms(1000);
    }

    // 5. Initialize SPI1 Telemetry Link
    Telemetry_SPI_Init();
    printf("SPI1 Telemetry Master Initialized.\r\n");

    // 6. Initialize the Advanced DSP Engine
    DSP_Init(&dsp);
    printf("Advanced DSP Engine Initialized (Median -> IIR -> 2D Kalman)\r\n");
    
    // 7. Initialize GSM Module and Button
    GSM_Button_Init();
    GSM_UART_Init();
    printf("GSM Module and Button Initialized.\r\n");
    printf("--------------------------------------------------\r\n");

    // 8. Initialize Independent Watchdog
    IWDG_Handle_t IWDGHandle;
    IWDGHandle.pIWDG = IWDG;
    IWDGHandle.IWDG_Config.IWDG_Prescaler = IWDG_PR_DIV_256;
    IWDGHandle.IWDG_Config.IWDG_Reload = BOARD_IWDG_RELOAD_VAL; // 2500 * (256/32000) = 20 seconds
    IWDG_Init(&IWDGHandle);
    printf("IWDG Initialized with 20s timeout.\r\n");

    // 9. Initialize Baremetal RTOS Kernel and App Tasks
    Kernel_Init();
    AppTasks_Init();

    printf("Starting Baremetal RTOS Kernel...\r\n");
    
    // Enter the RTOS Preemptive Kernel
    Kernel_Start();

    return 0;
}
