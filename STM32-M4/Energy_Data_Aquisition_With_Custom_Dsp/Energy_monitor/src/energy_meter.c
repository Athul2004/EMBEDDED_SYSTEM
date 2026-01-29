/*
 * energy_meter.c
 * Core Logic for Energy Monitor
 * MISRA Refactored
 */

#include "energy_meter.h"
#include "adc_dma_driver.h"
#include "uart_driver.h"
#include "i2c_driver.h"
#include "timer_driver.h"
#include "ssd1306.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

// --- CONSTANTS ---
#define BUF_LEN             128U        // ADC Buffer Size
#define V_OFFSET            2065        // Voltage Offset
#define I_OFFSET            2045        // Current Offset
#define SAMPLES_PER_SEC     8000        // Sampling Rate
#define NOISE_THRES_V       20.0f       // Voltage Noise Threshold
#define NOISE_THRES_I       0.05f       // Current Noise Threshold
#define ZERO_CROSS_THRES    100         // Zero Crossing Hysteresis

// --- CALIBRATION FACTORS ---
static const float CAL_V = 0.727f;
static const float CAL_I = 0.0136f;

// --- BUFFERS ---
static uint32_t adc_buffer[BUF_LEN];

// --- STATIC Prototypes ---
static void Hardware_Init(void);
static void Accumulate_Data(int32_t start_index);
static void Update_Display_And_Log(float v_rms, float i_rms, float active_power, float energy_kwh, float pf, float frequency);

// Function to Initialize the Energy Meter Application
void EnergyMeter_Init(void) {
    Hardware_Init(); // Initialize peripherals including UART
    
    // OLED Startup Sequence
    SSD1306_Init();
    SSD1306_Clear();
    SSD1306_PrintCentered(2, "ENERGY METER");
    SSD1306_PrintCentered(4, "STARTING...");
    SSD1306_Update();

    UART2_SendString("System Online.\r\n");
}

// Main Application Loop
void EnergyMeter_Run(void) {
    // Check Half Transfer Flag
    if ((DMA2->LISR & (1U << 4)) != 0U) {
        DMA2->LIFCR |= (1U << 4);
        Accumulate_Data(0); 
    }
    
    // Check Transfer Complete Flag
    if ((DMA2->LISR & (1U << 5)) != 0U) {
        DMA2->LIFCR |= (1U << 5);
        Accumulate_Data((int32_t)(BUF_LEN / 2U));
    }
}

static void Hardware_Init(void) {
    FPU_CPACR |= (0xFU << 20); // Enable FPU
    
    I2C1_Init();
    UART2_Init();
    TIM2_Init();
    ADC_DMA_Init(adc_buffer, BUF_LEN); 
}

static void Accumulate_Data(int32_t start_index) {
    static uint64_t acc_v_sq = 0;
    static uint64_t acc_i_sq = 0;
    static float acc_p_inst = 0.0f;
    static int32_t sample_count = 0;
    static int32_t last_v_sign = 0;
    static int32_t zero_crossings = 0;
    static float energy_ws = 0.0f;

    int32_t limit = start_index + (int32_t)(BUF_LEN / 2U);

    for(int32_t n = start_index; n < limit; n += 2) {
        int32_t v = (int32_t)adc_buffer[n] - V_OFFSET;
        int32_t i = (int32_t)adc_buffer[n+1] - I_OFFSET;

        acc_v_sq += (uint64_t)(v * v);
        acc_i_sq += (uint64_t)(i * i);
        
        // P = -V * I (Sensor polarity)
        acc_p_inst += (float)(-(v * i)); 

        // Frequency Detection
        if (abs(v) > ZERO_CROSS_THRES) {
            int32_t current_sign = (v > 0) ? 1 : -1;
            if((current_sign != last_v_sign) && (last_v_sign != 0)) {
                zero_crossings++;
            }
            last_v_sign = current_sign;
        }
        sample_count++;
    }

    if (sample_count >= SAMPLES_PER_SEC) {
        float v_rms = sqrtf((float)acc_v_sq / (float)sample_count) * CAL_V;
        float i_rms = sqrtf((float)acc_i_sq / (float)sample_count) * CAL_I;

        if (v_rms < NOISE_THRES_V) {
            v_rms = 0.0f; 
            i_rms = 0.0f; 
            zero_crossings = 0;
        }
        if (i_rms < NOISE_THRES_I) {
            i_rms = 0.0f;
        }

        float active_power = (acc_p_inst / (float)sample_count) * CAL_V * CAL_I;
        if (i_rms == 0.0f) { active_power = 0.0f; }
        if (active_power < 0.0f) { active_power = -active_power; }

        float apparent_power = v_rms * i_rms;
        float pf = 0.0f;
        if (apparent_power > 0.5f) {
            pf = (active_power / apparent_power) * 100.0f;
            if (pf > 100.0f) { pf = 100.0f; }
        }

        float frequency = (float)zero_crossings / 2.0f;
        
        energy_ws += active_power; // 1 sec window
        float energy_kwh = energy_ws / 3600000.0f;

        Update_Display_And_Log(v_rms, i_rms, active_power, energy_kwh, pf, frequency);

        // Reset
        acc_v_sq = 0; 
        acc_i_sq = 0; 
        acc_p_inst = 0.0f; 
        sample_count = 0;
        zero_crossings = 0;
    }
}

static void Update_Display_And_Log(float v_rms, float i_rms, float active_power, float energy_kwh, float pf, float frequency) {
    SSD1306_Clear();
    SSD1306_PrintCentered(0, "ENERGY METER");

    int i_int = (int)i_rms;
    int i_dec = (int)((i_rms - (float)i_int) * 100.0f);

    SSD1306_SetCursor(8, 2);
    SSD1306_Print("V:"); SSD1306_PrintNumber((int)v_rms);

    SSD1306_SetCursor(70, 2);
    SSD1306_Print("A:"); SSD1306_PrintNumber(i_int);
    SSD1306_Print("."); if(i_dec<10) { SSD1306_Print("0"); } SSD1306_PrintNumber(i_dec);

    int e_int = (int)energy_kwh;
    int e_dec = (int)((energy_kwh - (float)e_int) * 1000.0f);

    SSD1306_SetCursor(8, 4);
    SSD1306_Print("W:"); SSD1306_PrintNumber((int)active_power);

    SSD1306_SetCursor(70, 4);
    SSD1306_Print("E:"); SSD1306_PrintNumber(e_int);
    SSD1306_Print(".");
    if(e_dec<100) { SSD1306_Print("0"); }
    if(e_dec<10) { SSD1306_Print("0"); }
    SSD1306_PrintNumber(e_dec);

    SSD1306_SetCursor(8, 6);
    SSD1306_Print("PF:");
    if (pf >= 99.9f) { SSD1306_Print("1.00"); }
    else {
        SSD1306_Print("0.");
        if(pf < 10.0f) { SSD1306_Print("0"); }
        SSD1306_PrintNumber((int)pf);
    }

    SSD1306_SetCursor(70, 6);
    SSD1306_Print("F:"); SSD1306_PrintNumber((int)frequency);

    SSD1306_Update();

    // UART LOGGING
    UART2_SendString("\r\n--- UPDATE ---\r\n");
    UART2_SendString("V: "); UART2_SendNumber((int)v_rms);
    UART2_SendString("| I: "); UART2_SendNumber(i_int); UART2_SendString("."); if(i_dec<10) {UART2_SendString("0");} UART2_SendNumber(i_dec);
    UART2_SendString("| W: "); UART2_SendNumber((int)active_power);
    UART2_SendString("| E: "); UART2_SendNumber(e_int); UART2_SendString("."); if(e_dec<100) {UART2_SendString("0");} if(e_dec<10) {UART2_SendString("0");} UART2_SendNumber(e_dec);
    UART2_SendString("| PF: "); UART2_SendNumber((int)pf);
    UART2_SendString("| F: "); UART2_SendNumber((int)frequency);
    UART2_SendString("\r\n");
}
