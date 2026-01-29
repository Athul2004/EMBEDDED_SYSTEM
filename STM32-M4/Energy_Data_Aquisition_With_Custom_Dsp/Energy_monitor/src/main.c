/**
 * @file main.c
 * @brief Main entry point for the Energy Monitor Application
 * MISRA Refactored
 */

#include "stm32_f446xx.h"   // Include hardware definitions and mapping
#include "energy_meter.h"   // Include Application Logic Header

/*
 * @brief  Main program execution entry point
 * @param  None
 * @retval int (Standard C main return type, though typically void in embedded)
 */
int main(void)
{
    // Initialize the Energy Meter Application
    // This sets up Clocks, GPIOs, ADC, DMA, Timer, UART, I2C, and OLED
    EnergyMeter_Init();

    // Infinite Main Loop
    // The system logic is event-driven by interrupts (DMA flags), polled here
    while(1)
    {
        // Continuous check for data acquisition events
        // If buffer is full/half-full, processing happens inside this function
        EnergyMeter_Run();
    }
}
