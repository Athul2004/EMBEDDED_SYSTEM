/**
 * @file main.c
 * @brief Main entry point for the Energy Monitor Application
 */

#include "stm32_f446xx.h"       // Device Header
#include "energy_meter.h"       // Application Logic Header

/*
 * @brief  Application Entry Point
 * @retval int
 */
int main(void)
{
    // Initialize the Energy Meter Application
    // This internally initializes:
    // - Hardware Clocks, GPIOs, ADC, DMA, Timers, UART, I2C
    // - OLED Display
    // - Application State
    EnergyMeter_Init();

    // Infinite Loop
    while(1)
    {
        // Run the main application task
        // Checks for new data from DMA and processes it
        EnergyMeter_Run();
    }
}
