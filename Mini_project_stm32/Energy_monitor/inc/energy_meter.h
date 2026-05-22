/*
 * energy_meter.h
 * Energy Monitor Application Header
 */

#ifndef ENERGY_METER_H_
#define ENERGY_METER_H_

#include "stm32_f446xx.h"    // Include hardware definitions

// Function prototype to initialize the Energy Meter application and peripherals
void EnergyMeter_Init(void);

// Function prototype for the main application loop of the Energy Meter
void EnergyMeter_Run(void);

#endif /* ENERGY_METER_H_ */
