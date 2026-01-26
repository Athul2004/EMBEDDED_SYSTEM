/*
 * energy_meter.c
 * Core Logic for Energy Monitor
 */

#include "energy_meter.h"       // Application header
#include "stm32_f446xx.h"       // Device header
#include "adc_dma_driver.h"     // ADC Driver header
#include "uart_driver.h"        // UART Driver header
#include "i2c_driver.h"         // I2C Driver header
#include "timer_driver.h"       // Timer Driver header
#include "ssd1306.h"            // Display Driver header
#include <math.h>               // Math library for sqrtf, abs
#include <stdlib.h>             // Standard library
#include <string.h>             // String library

// --- BUFFERS ---
#define BUF_LEN 128             // Define buffer length for ADC data (128 samples)
uint32_t adc_buffer[BUF_LEN];   // Buffer to store ADC values (interleaved Ch0/Ch1)

// --- CALIBRATION ---
#define V_OFFSET 2065           // Voltage offset (ADC zero point ~ Vref/2)
#define I_OFFSET 2045           // Current offset (ADC zero point ~ Vref/2)
float v_cal = 0.727f;           // Voltage calibration factor
float i_cal = 0.0136f;          // Current calibration factor

// --- INTERNAL PROTOTYPES ---
void Accumulate_Data(int start_index); // Forward declaration of analysis function

// Function to Initialize Hardware and Drivers
void Hardware_Init(void) {
    FPU_CPACR |= (0xF << 20);   // Enable Floating Point Unit (FPU) for fast math

    // Initialize Peripheral Drivers
    I2C1_Init();                // Initialize I2C1 for OLED
    UART2_Init();               // Initialize UART2 for Serial Logging
    TIM2_Init();                // Initialize Timer2 for ADC Triggering
    
    // Initialize ADC with DMA
    // Configures ADC1 to scan Ch0/Ch1 and use DMA2 Stream 0 to transfer data to buffer
    ADC_DMA_Init(adc_buffer, BUF_LEN); 
}

// Function to Initialize the Energy Meter Application
void EnergyMeter_Init(void) {
    Hardware_Init();            // Call hardware initialization
    SSD1306_Init();             // Initialize the SSD1306 OLED Display

    SSD1306_Clear();            // Clear the display buffer
    SSD1306_PrintCentered(2, "ENERGY METER"); // Print Title
    SSD1306_PrintCentered(4, "STARTING...");  // Print Status
    SSD1306_Update();           // Update the display to show changes

    UART2_SendString("System Online.\r\n"); // Send boot message over UART
}

// Main Application Loop Function
void EnergyMeter_Run(void) {
    // Poll DMA Transfer Flags to process data in halves (Double Buffering simulation)
    
    // Check Half Transfer Interrupt Flag (Stream 0, Bit 4 in LISR)
    if (DMA2->LISR & (1 << 4)) {
        DMA2->LIFCR |= (1 << 4);    // Clear Half Transfer Flag
        Accumulate_Data(0);         // Process first half of the buffer (0 to 63)
    }
    
    // Check Transfer Complete Interrupt Flag (Stream 0, Bit 5 in LISR)
    if (DMA2->LISR & (1 << 5)) {
        DMA2->LIFCR |= (1 << 5);    // Clear Transfer Complete Flag
        Accumulate_Data(BUF_LEN / 2); // Process second half of the buffer (64 to 127)
    }
}

// Function to Process ADC Data and Calculate Energy Parameters
// start_index: Index in buffer to start processing (0 for 1st half, 64 for 2nd half)
void Accumulate_Data(int start_index) {
    // Static variables to maintain state across function calls (Accumulators)
    static uint64_t acc_v_sq = 0;   // Accumulated Variance of Voltage (Sum of V^2)
    static uint64_t acc_i_sq = 0;   // Accumulated Variance of Current (Sum of I^2)
    static float acc_p_inst = 0;    // Accumulated Instantaneous Power (Sum of V*I) - Float from wrapper
    static int sample_count = 0;    // Counter for number of samples processed

    // Frequency Calculation State
    static int last_v_sign = 0;     // Sign of voltage in previous sample (+1 or -1)
    static int zero_crossings = 0;  // Counter for zero crossings detected

    // Energy Accumulation (Watt-Seconds)
    static float energy_ws = 0.0f;  // Total energy in Watt-Seconds

    int limit = start_index + (BUF_LEN / 2); // Calculate end index for current batch

    // Loop through the assigned buffer section
    // Step by 2 because buffer is interleaved (V, I, V, I...)
    for(int n = start_index; n < limit; n += 2) {
        // Read and center raw ADC values (remove DC offset)
        int32_t v = (int32_t)adc_buffer[n] - V_OFFSET;     // Voltage sample
        int32_t i = (int32_t)adc_buffer[n+1] - I_OFFSET;   // Current sample

        // Accumulate Squares for RMS calculation
        acc_v_sq += (v * v);        // V^2
        acc_i_sq += (i * i);        // I^2
        
        // Accumulate Instantaneous Power
        // P = V * I. Negative sign due to sensor direction/polarity.
        acc_p_inst += (float)(-(v * i)); 

        // Frequency Detection (Zero Crossing) with Hysteresis
        if (abs(v) > 100) { // Noise threshold: Ignore small voltage fluctuations near zero
            int current_sign = (v > 0) ? 1 : -1; // Determine current sign
            // If sign changed and not just starting up
            if(current_sign != last_v_sign && last_v_sign != 0) {
                zero_crossings++; // Count a zero crossing
            }
            last_v_sign = current_sign; // Update last sign
        }
        sample_count++; // Increment total sample counter
    }

    // Check if we have accumulated enough samples for a reading (approx 1 second)
    // 8000 samples @ 8kHz = 1 second
    if (sample_count >= 8000) { 
        // --- 1. CALCULATIONS ---
        
        // RMS Voltage = sqrt(Sum(V^2)/N) * Calibration Factor
        float v_rms = sqrtf((float)acc_v_sq / sample_count) * v_cal;
        
        // RMS Current = sqrt(Sum(I^2)/N) * Calibration Factor
        float i_rms = sqrtf((float)acc_i_sq / sample_count) * i_cal;

        // --- NOISE GATING ---
        // If Voltage is too low (unplugged?), zero out everything
        if (v_rms < 20.0f) {
            v_rms = 0.0f; i_rms = 0.0f; zero_crossings = 0;
        }
        // If Current is very low (noise level), zero it
        if (i_rms < 0.05f) i_rms = 0.0f;

        // Active Power = (Sum(P_inst) / N) * V_cal * I_cal
        float active_power = (acc_p_inst / sample_count) * v_cal * i_cal;
        
        // Sanity check: If current is zero, power must be zero
        if (i_rms == 0.0f) active_power = 0.0f;
        
        // Ensure positive power display (absolute value)
        if (active_power < 0) active_power = -active_power;

        // Apparent Power = V_rms * I_rms
        float apparent_power = v_rms * i_rms;

        // Power Factor = Active Power / Apparent Power
        float pf = 0.0f;
        if (apparent_power > 0.5f) { // Avoid division by zero
            pf = (active_power / apparent_power) * 100.0f; // As percentage
            if (pf > 100.0f) pf = 100.0f; // Cap at 100%
        }

        // Frequency = Zero Crossings / 2 (2 crossings per cycle)
        float frequency = (float)zero_crossings / 2.0f;

        // --- ENERGY ACCUMULATION (KWh) ---
        // Add active power (Watts) to energy accumulator (Watt-Seconds)
        // Since update is approx 1 sec, samples are roughly energy in Joules/Sec * Sec
        // Actually: Power is Average for this 1s. Energy = Power * Time (1s).
        // So we just add active_power to accumulator.
        energy_ws += active_power;
        
        // Convert Watt-Seconds to Kilowatt-Hours
        // 1 kWh = 1000 W * 3600 s = 3,600,000 Ws
        float energy_kwh = energy_ws / 3600000.0f; 

        // --- 2. OLED DISPLAY (With kWh) ---
        SSD1306_Clear(); // Clear screen for new update

        // ROW 0: TITLE
        SSD1306_PrintCentered(0, "ENERGY METER");

        // ROW 2: Voltage | Current
        int i_int = (int)i_rms; // Integer part of Current
        int i_dec = (int)((i_rms - i_int) * 100); // Decimal part (2 digits)

        SSD1306_SetCursor(8, 2);
        SSD1306_Print("V:"); SSD1306_PrintNumber((int)v_rms); // Print Voltage

        SSD1306_SetCursor(70, 2);
        SSD1306_Print("A:"); SSD1306_PrintNumber(i_int);
        SSD1306_Print("."); if(i_dec<10) SSD1306_Print("0"); SSD1306_PrintNumber(i_dec); // Print Current

        // ROW 4: Active Power | Energy (kWh)
        int e_int = (int)energy_kwh; // Integer part of Energy
        int e_dec = (int)((energy_kwh - e_int) * 1000); // Decimal part (3 digits)

        SSD1306_SetCursor(8, 4);
        SSD1306_Print("W:"); SSD1306_PrintNumber((int)active_power); // Print Power

        SSD1306_SetCursor(70, 4);
        SSD1306_Print("E:"); SSD1306_PrintNumber(e_int);
        SSD1306_Print(".");
        if(e_dec<100) SSD1306_Print("0");
        if(e_dec<10) SSD1306_Print("0");
        SSD1306_PrintNumber(e_dec); // Print Energy

        // ROW 6: Power Factor | Frequency
        SSD1306_SetCursor(8, 6);
        SSD1306_Print("PF:");
        if (pf >= 99.9f) SSD1306_Print("1.00");
        else {
            SSD1306_Print("0.");
            if(pf < 10) SSD1306_Print("0");
            SSD1306_PrintNumber((int)pf); // Print PF
        }

        SSD1306_SetCursor(70, 6);
        SSD1306_Print("F:"); SSD1306_PrintNumber((int)frequency); // Print Freq

        SSD1306_Update(); // Send buffer to screen

        // --- 3. UART LOGGING (ALL PARAMETERS) ---
        UART2_SendString("\r\n--- UPDATE ---\r\n");

        // Send Voltage
        UART2_SendString("V: "); UART2_SendNumber((int)v_rms);

        // Send Current
        UART2_SendString(" | I: "); UART2_SendNumber(i_int); UART2_SendString("."); if(i_dec<10) UART2_SendString("0"); UART2_SendNumber(i_dec); UART2_SendString("A");

        // Send Power
        UART2_SendString(" | W: "); UART2_SendNumber((int)active_power);

        // Send Energy
        UART2_SendString(" | kWh: "); UART2_SendNumber(e_int); UART2_SendString(".");
        if(e_dec<100) UART2_SendString("0"); if(e_dec<10) UART2_SendString("0"); UART2_SendNumber(e_dec);

        // Send Power Factor
        UART2_SendString(" | PF: ");
        if (pf >= 99.9f) UART2_SendString("1.00");
        else {
            UART2_SendString("0.");
            if(pf < 10) UART2_SendString("0");
            UART2_SendNumber((int)pf);
        }

        // Send Frequency
        UART2_SendString(" | F: "); UART2_SendNumber((int)frequency); UART2_SendString("Hz");

        UART2_SendString("\r\n");

        // Reset Accumulators for next 1-second window
        acc_v_sq = 0; acc_i_sq = 0; acc_p_inst = 0; sample_count = 0;
        zero_crossings = 0;
    }
}
