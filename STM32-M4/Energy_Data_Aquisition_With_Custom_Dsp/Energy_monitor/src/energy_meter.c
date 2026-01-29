/*
 * energy_meter.c
 * Core Logic for Energy Monitor
 * MISRA Refactored
 */

#include "energy_meter.h"       // Include application header
#include "adc_dma_driver.h"     // Include ADC/DMA driver for data acquisition
#include "uart_driver.h"        // Include UART driver for data logging
#include "i2c_driver.h"         // Include I2C driver for display communication
#include "timer_driver.h"       // Include Timer driver for periodic sampling
#include "ssd1306.h"            // Include OLED driver for display output
#include <math.h>               // Include math library for sqrtf, fabs
#include <stdlib.h>             // Include standard library
#include <string.h>             // Include string manipulation library

// --- CONSTANTS ---
#define BUF_LEN             128U        // ADC Buffer Size (Power of 2 ideal for halves)
#define V_OFFSET            2065        // Voltage Sensor DC Offset (calibrated value)
#define I_OFFSET            2045        // Current Sensor DC Offset (calibrated value)
#define SAMPLES_PER_SEC     8000        // Expected Sampling Rate in Hz
#define NOISE_THRES_V       20.0f       // Voltage Noise Threshold below which V=0
#define NOISE_THRES_I       0.05f       // Current Noise Threshold below which I=0
#define ZERO_CROSS_THRES    100         // Zero Crossing Hysteresis threshold in ADC counts

// --- CALIBRATION FACTORS ---
static const float CAL_V = 0.727f;      // Voltage calibration multiplier to get Volts
static const float CAL_I = 0.0136f;     // Current calibration multiplier to get Amps

// --- BUFFERS ---
static uint32_t adc_buffer[BUF_LEN];    // DMA destination buffer for raw ADC values (interleaved)

// --- STATIC Prototypes ---
static void Hardware_Init(void);        // Internal function to initialize hardware
static void Accumulate_Data(int32_t start_index); // Internal function to process a batch of data
// Internal function to update display and send UART logs
static void Update_Display_And_Log(float v_rms, float i_rms, float active_power, float energy_kwh, float pf, float frequency);

// Function to Initialize the Energy Meter Application
void EnergyMeter_Init(void) {
    Hardware_Init(); // Identify and initialize all required peripherals
    
    // OLED Startup Sequence
    SSD1306_Init();     // Initialize the OLED display controller
    SSD1306_Clear();    // Clear any random garbage from display memory
    SSD1306_PrintCentered(2, "ENERGY METER");   // Print Title centered on line 2
    SSD1306_PrintCentered(4, "STARTING...");    // Print Status centered on line 4
    SSD1306_Update();   // Send buffer to physical display to show text

    UART2_SendString("System Online.\r\n");     // Send boot message via UART
}

// Main Application Loop
void EnergyMeter_Run(void) {
    // Check Half Transfer Flag (HTIF4) in DMA2 Stream 0 Interrupt Status Register (LISR)
    // Bit 4 corresponds to Half Transfer for Stream 0
    if ((DMA2->LISR & (1U << 4)) != 0U) {
        DMA2->LIFCR |= (1U << 4);   // Clear the Half Transfer Interrupt Flag
        Accumulate_Data(0);         // Process the first half of the buffer (indices 0 to BUF_LEN/2 - 1)
    }
    
    // Check Transfer Complete Flag (TCIF5) in DMA2 Stream 0 Interrupt Status Register (LISR)
    // Bit 5 corresponds to Transfer Complete for Stream 0
    if ((DMA2->LISR & (1U << 5)) != 0U) {
        DMA2->LIFCR |= (1U << 5);   // Clear the Transfer Complete Interrupt Flag
        Accumulate_Data((int32_t)(BUF_LEN / 2U)); // Process second half (indices BUF_LEN/2 to BUF_LEN - 1)
    }
}

// Internal Hardware Initialization
static void Hardware_Init(void) {
    FPU_CPACR |= (0xFU << 20); // Enable FPU (Floating Point Unit) by setting CP10 and CP11 to Full Access
    
    I2C1_Init();        // Initialize I2C peripheral for OLED
    UART2_Init();       // Initialize UART peripheral for Logging
    TIM2_Init();        // Initialize Timer for ADC triggering
    ADC_DMA_Init(adc_buffer, BUF_LEN); // Initialize ADC and DMA with the buffer
}

// Data Processing Function
static void Accumulate_Data(int32_t start_index) {
    // Static variables to persist state between function calls
    static uint64_t acc_v_sq = 0;       // Accumulator for Voltage Squared (for RMS)
    static uint64_t acc_i_sq = 0;       // Accumulator for Current Squared (for RMS)
    static float acc_p_inst = 0.0f;     // Accumulator for Instantaneous Power
    static int32_t sample_count = 0;    // Counter for number of samples processed
    static int32_t last_v_sign = 0;     // Sign of voltage in previous sample (for zero-crossing)
    static int32_t zero_crossings = 0;  // Counter for zero crossings detected
    static float energy_ws = 0.0f;      // Accumulated energy in Watt-Seconds

    // Calculate the limit index for this processing batch
    int32_t limit = start_index + (int32_t)(BUF_LEN / 2U);

    // Iterate through the buffer chunk
    // Step by 2 because data is interleaved: [V0, I0, V1, I1, ...]
    for(int32_t n = start_index; n < limit; n += 2) {
        // Read Raw Voltage and subtract offset to get AC component
        int32_t v = (int32_t)adc_buffer[n] - V_OFFSET;
        // Read Raw Current and subtract offset to get AC component
        int32_t i = (int32_t)adc_buffer[n+1] - I_OFFSET;

        // Accumulate squares for RMS calculation
        acc_v_sq += (uint64_t)(v * v);
        acc_i_sq += (uint64_t)(i * i);
        
        // Calculate Instantaneous Power: P = V * I. 
        // Note: -V * I corrects for sensor polarity in hardware installation
        acc_p_inst += (float)(-(v * i)); 

        // Frequency Detection Logic (Zero-Crossing)
        // Check if voltage magnitude exceeds hysteresis threshold to avoid noise
        if (abs(v) > ZERO_CROSS_THRES) {
            // Determine current sign: positive (1) or negative (-1)
            int32_t current_sign = (v > 0) ? 1 : -1;
            // Detect sign change (transition from + to - or - to +)
            if((current_sign != last_v_sign) && (last_v_sign != 0)) {
                zero_crossings++;   // Increment zero crossing counter
            }
            last_v_sign = current_sign; // Update last sign state
        }
        sample_count++; // Increment total sample counter
    }

    // Check if we have accumulated 1 second worth of data (8000 samples)
    if (sample_count >= SAMPLES_PER_SEC) {
        // Calculate RMS Voltage: sqrt(mean of squares) * Calibration Factor
        float v_rms = sqrtf((float)acc_v_sq / (float)sample_count) * CAL_V;
        // Calculate RMS Current: sqrt(mean of squares) * Calibration Factor
        float i_rms = sqrtf((float)acc_i_sq / (float)sample_count) * CAL_I;

        // Apply Noise Thresholds (Zero-out readings if below noise floor)
        if (v_rms < NOISE_THRES_V) {
            v_rms = 0.0f; 
            i_rms = 0.0f; // If voltage is zero, current implies noise usually
            zero_crossings = 0; // No voltage means no frequency
        }
        if (i_rms < NOISE_THRES_I) {
            i_rms = 0.0f;
        }

        // Calculate Active Power: Mean of instantaneous power * Calibration Factors
        float active_power = (acc_p_inst / (float)sample_count) * CAL_V * CAL_I;
        
        // Final sanity checks on power
        if (i_rms == 0.0f) { active_power = 0.0f; } // No current flow means no power
        if (active_power < 0.0f) { active_power = -active_power; } // Absolute value for display

        // Calculate Apparent Power: V_rms * I_rms
        float apparent_power = v_rms * i_rms;
        
        // Calculate Power Factor: Active Power / Apparent Power
        float pf = 0.0f;
        if (apparent_power > 0.5f) { // Avoid division by near-zero
            pf = (active_power / apparent_power) * 100.0f; // In percentage
            if (pf > 100.0f) { pf = 100.0f; } // Cap at 100%
        }

        // Calculate Frequency: Zero Crossings / 2 (since 2 crossings per cycle)
        float frequency = (float)zero_crossings / 2.0f;
        
        // Accumulate Energy
        energy_ws += active_power; // Since window is 1 sec, Power * 1s = Energy in Joules (Ws)
        float energy_kwh = energy_ws / 3600000.0f; // Convert Ws to kWh (1000 * 3600)

        // Update the User Interface and Logs
        Update_Display_And_Log(v_rms, i_rms, active_power, energy_kwh, pf, frequency);

        // Reset accumulators for the next 1-second window
        acc_v_sq = 0; 
        acc_i_sq = 0; 
        acc_p_inst = 0.0f; 
        sample_count = 0;
        zero_crossings = 0;
    }
}

// Function to update OLED and UART
static void Update_Display_And_Log(float v_rms, float i_rms, float active_power, float energy_kwh, float pf, float frequency) {
    SSD1306_Clear();    // Clear display buffer
    SSD1306_PrintCentered(0, "ENERGY METER"); // Print Header

    // Format Current for display (Integers and Decimals manually for embedded usage)
    int i_int = (int)i_rms;
    int i_dec = (int)((i_rms - (float)i_int) * 100.0f); // 2 decimal places

    // Display Voltage
    SSD1306_SetCursor(8, 2);
    SSD1306_Print("V:"); SSD1306_PrintNumber((int)v_rms);

    // Display Current
    SSD1306_SetCursor(70, 2);
    SSD1306_Print("A:"); SSD1306_PrintNumber(i_int);
    SSD1306_Print("."); if(i_dec<10) { SSD1306_Print("0"); } SSD1306_PrintNumber(i_dec);

    // Format Energy
    int e_int = (int)energy_kwh;
    int e_dec = (int)((energy_kwh - (float)e_int) * 1000.0f); // 3 decimal places

    // Display Power (Watts)
    SSD1306_SetCursor(8, 4);
    SSD1306_Print("W:"); SSD1306_PrintNumber((int)active_power);

    // Display Accumulated Energy (kWh)
    SSD1306_SetCursor(70, 4);
    SSD1306_Print("E:"); SSD1306_PrintNumber(e_int);
    SSD1306_Print(".");
    if(e_dec<100) { SSD1306_Print("0"); }   // Zero padding for decimal
    if(e_dec<10) { SSD1306_Print("0"); }
    SSD1306_PrintNumber(e_dec);

    // Display Power Factor
    SSD1306_SetCursor(8, 6);
    SSD1306_Print("PF:");
    if (pf >= 99.9f) { SSD1306_Print("1.00"); }
    else {
        SSD1306_Print("0.");
        if(pf < 10.0f) { SSD1306_Print("0"); }
        SSD1306_PrintNumber((int)pf);
    }

    // Display Frequency
    SSD1306_SetCursor(70, 6);
    SSD1306_Print("F:"); SSD1306_PrintNumber((int)frequency);

    SSD1306_Update();   // Send buffer to OLED

    // UART LOGGING (Send plain text via Serial)
    UART2_SendString("\r\n--- UPDATE ---\r\n");
    UART2_SendString("V: "); UART2_SendNumber((int)v_rms);
    UART2_SendString("| I: "); UART2_SendNumber(i_int); UART2_SendString("."); if(i_dec<10) {UART2_SendString("0");} UART2_SendNumber(i_dec);
    UART2_SendString("| W: "); UART2_SendNumber((int)active_power);
    UART2_SendString("| E: "); UART2_SendNumber(e_int); UART2_SendString("."); if(e_dec<100) {UART2_SendString("0");} if(e_dec<10) {UART2_SendString("0");} UART2_SendNumber(e_dec);
    UART2_SendString("| PF: "); UART2_SendNumber((int)pf);
    UART2_SendString("| F: "); UART2_SendNumber((int)frequency);
    UART2_SendString("\r\n");
}
