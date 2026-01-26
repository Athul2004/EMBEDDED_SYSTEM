/*
 * energy_meter.c
 * Core Logic for Energy Monitor
 */

#include "energy_meter.h"
#include "stm32_f446xx.h"
#include "adc_dma_driver.h"
#include "uart_driver.h"
#include "i2c_driver.h"
#include "timer_driver.h"
#include "ssd1306.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

// --- BUFFERS ---
#define BUF_LEN 128
uint32_t adc_buffer[BUF_LEN];

// --- CALIBRATION ---
#define V_OFFSET 2065
#define I_OFFSET 2045
float v_cal = 0.727f;
float i_cal = 0.0136f;

// --- INTERNAL PROTOTYPES ---
void Accumulate_Data(int start_index);

void Hardware_Init(void) {
    FPU_CPACR |= (0xF << 20); // Enable FPU

    // Initialize Drivers
    I2C1_Init();
    UART2_Init();
    TIM2_Init();
    
    // ADC DMA Init (Logic from snippet moved to driver, called here)
    ADC_DMA_Init(adc_buffer, BUF_LEN);
}

void EnergyMeter_Init(void) {
    Hardware_Init();
    SSD1306_Init();

    SSD1306_Clear();
    SSD1306_PrintCentered(2, "ENERGY METER");
    SSD1306_PrintCentered(4, "STARTING...");
    SSD1306_Update();

    UART2_SendString("System Online.\r\n");
}

void EnergyMeter_Run(void) {
    if (DMA2->LISR & (1 << 4)) {
        DMA2->LIFCR |= (1 << 4);
        Accumulate_Data(0);
    }
    if (DMA2->LISR & (1 << 5)) {
        DMA2->LIFCR |= (1 << 5);
        Accumulate_Data(BUF_LEN / 2);
    }
}

// User-provided Accumulate_Data (Exact Snippet Logic)
void Accumulate_Data(int start_index) {
    static uint64_t acc_v_sq = 0;
    static uint64_t acc_i_sq = 0;
    static float acc_p_inst = 0;
    static int sample_count = 0;

    // Frequency Calculation
    static int last_v_sign = 0;
    static int zero_crossings = 0;

    // Energy Accumulation (Watt-Seconds)
    static float energy_ws = 0.0f; // Stores Watt-Seconds

    int limit = start_index + (BUF_LEN / 2);

    for(int n = start_index; n < limit; n += 2) {
        int32_t v = (int32_t)adc_buffer[n] - V_OFFSET;
        int32_t i = (int32_t)adc_buffer[n+1] - I_OFFSET;

        acc_v_sq += (v * v);
        acc_i_sq += (i * i);
        acc_p_inst += (float)(-(v * i));

        // Hysteresis for Frequency
        if (abs(v) > 100) {
            int current_sign = (v > 0) ? 1 : -1;
            if(current_sign != last_v_sign && last_v_sign != 0) {
                zero_crossings++;
            }
            last_v_sign = current_sign;
        }
        sample_count++;
    }

    if (sample_count >= 8000) { // Approx 1.0 second update rate
        // --- 1. CALCULATIONS ---
        float v_rms = sqrtf((float)acc_v_sq / sample_count) * v_cal;
        float i_rms = sqrtf((float)acc_i_sq / sample_count) * i_cal;

        // --- NOISE GATING ---
        if (v_rms < 20.0f) {
            v_rms = 0.0f; i_rms = 0.0f; zero_crossings = 0;
        }
        if (i_rms < 0.05f) i_rms = 0.0f;

        float active_power = (acc_p_inst / sample_count) * v_cal * i_cal;
        if (i_rms == 0.0f) active_power = 0.0f;
        if (active_power < 0) active_power = -active_power;

        float apparent_power = v_rms * i_rms;

        // Power Factor
        float pf = 0.0f;
        if (apparent_power > 0.5f) {
            pf = (active_power / apparent_power) * 100.0f;
            if (pf > 100.0f) pf = 100.0f;
        }

        float frequency = (float)zero_crossings / 2.0f;

        // --- ENERGY ACCUMULATION (KWh) ---
        energy_ws += active_power;
        float energy_kwh = energy_ws / 3600000.0f; // Convert Ws -> kWh

        // --- 2. OLED DISPLAY (With kWh) ---
        SSD1306_Clear();

        // ROW 0: TITLE
        SSD1306_PrintCentered(0, "ENERGY METER");

        // ROW 2: Voltage | Current
        int i_int = (int)i_rms;
        int i_dec = (int)((i_rms - i_int) * 100);

        SSD1306_SetCursor(8, 2);
        SSD1306_Print("V:"); SSD1306_PrintNumber((int)v_rms);

        SSD1306_SetCursor(70, 2);
        SSD1306_Print("A:"); SSD1306_PrintNumber(i_int);
        SSD1306_Print("."); if(i_dec<10) SSD1306_Print("0"); SSD1306_PrintNumber(i_dec);

        // ROW 4: Active Power | Energy (kWh)
        int e_int = (int)energy_kwh;
        int e_dec = (int)((energy_kwh - e_int) * 1000);

        SSD1306_SetCursor(8, 4);
        SSD1306_Print("W:"); SSD1306_PrintNumber((int)active_power);

        SSD1306_SetCursor(70, 4);
        SSD1306_Print("E:"); SSD1306_PrintNumber(e_int);
        SSD1306_Print(".");
        if(e_dec<100) SSD1306_Print("0");
        if(e_dec<10) SSD1306_Print("0");
        SSD1306_PrintNumber(e_dec);

        // ROW 6: Power Factor | Frequency
        SSD1306_SetCursor(8, 6);
        SSD1306_Print("PF:");
        if (pf >= 99.9f) SSD1306_Print("1.00");
        else {
            SSD1306_Print("0.");
            if(pf < 10) SSD1306_Print("0");
            SSD1306_PrintNumber((int)pf);
        }

        SSD1306_SetCursor(70, 6);
        SSD1306_Print("F:"); SSD1306_PrintNumber((int)frequency);

        SSD1306_Update();

        // --- 3. UART LOGGING (ALL PARAMETERS) ---
        UART2_SendString("\r\n--- UPDATE ---\r\n");
        UART2_SendString("V: "); UART2_SendNumber((int)v_rms);
        UART2_SendString(" | I: "); UART2_SendNumber(i_int); UART2_SendString("."); if(i_dec<10) UART2_SendString("0"); UART2_SendNumber(i_dec); UART2_SendString("A");
        UART2_SendString(" | W: "); UART2_SendNumber((int)active_power);
        UART2_SendString(" | kWh: "); UART2_SendNumber(e_int); UART2_SendString(".");
        if(e_dec<100) UART2_SendString("0"); if(e_dec<10) UART2_SendString("0"); UART2_SendNumber(e_dec);
        UART2_SendString(" | PF: ");
        if (pf >= 99.9f) UART2_SendString("1.00");
        else {
            UART2_SendString("0.");
            if(pf < 10) UART2_SendString("0");
            UART2_SendNumber((int)pf);
        }
        UART2_SendString(" | F: "); UART2_SendNumber((int)frequency); UART2_SendString("Hz");
        UART2_SendString("\r\n");

        acc_v_sq = 0; acc_i_sq = 0; acc_p_inst = 0; sample_count = 0;
        zero_crossings = 0;
    }
}
