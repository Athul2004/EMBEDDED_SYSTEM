/**
 * @file main.c
 * @brief Main entry point for the Energy Monitor Application
 * MISRA Refactored
 */

#include "stm32_f446xx.h"
#include "energy_meter.h"

int main(void)
{
    // Initialize the Application
    EnergyMeter_Init();

    // Infinite Loop
    while(1)
    {
        EnergyMeter_Run();
    }
}
