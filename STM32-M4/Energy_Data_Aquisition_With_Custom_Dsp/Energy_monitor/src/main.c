/**
 * @file main.c
 * @brief Main entry point
 */

#include "stm32_f446xx.h"
#include "energy_meter.h"

int main(void)
{
    // FPU Enable handled in Hardware_Init or here?
    // Snippet had `FPU_CPACR |= (0xF << 20);` in main.
    // energy_meter.c's Hardware_Init also has it. Redundant but harmless.
    // I will call EnergyMeter_Init() which calls Hardware_Init().
    
    // Snippet Main:
    /*
    int main(void) {
        FPU_CPACR |= (0xF << 20); // Enable FPU
        Hardware_Init(); // This is effectively EnergyMeter_Init part 1
        SSD1306_Init(); // Part 2
        ... // Part 3
    }
    */
    // My EnergyMeter_Init calls Hardware_Init -> SSD1306_Init -> Clear/Print.
    // So main just needs to call EnergyMeter_Init then Loop.
    
    EnergyMeter_Init();

    while(1)
    {
        EnergyMeter_Run();
    }
}
