#include "dsp/dsp_engine.h"
#include <stddef.h>

void DSP_Init(DSP_Engine_t *engine) {
    // Cascade_Init parameters: filter, initial_value, iir_alpha, kalman_r, kalman_q_val, kalman_q_vel
    
    // Temperature: Changes very slowly, moderately noisy
    // Alpha=0.2, High R (slow trust in measurement), Low Q
    Cascade_Init(&engine->temp_filter, 25.0f, 0.2f, 0.5f, 0.001f, 0.0001f);
    
    // Pressure: Moderately stable, but subject to atmospheric shifts
    // Alpha=0.5, Medium R
    Cascade_Init(&engine->press_filter, 101325.0f, 0.5f, 2.0f, 0.1f, 0.01f);
    
    // Altitude: Noisy but we want responsive vertical speed
    // Alpha=0.8 (Fast IIR to not lag velocity), R=1.0, Q_val=0.01, Q_vel=0.001
    Cascade_Init(&engine->alt_filter, 0.0f, 0.8f, 1.0f, 0.01f, 0.001f);
    
    engine->temp_roc = 0.0f;
    engine->press_roc = 0.0f;
    engine->vertical_speed = 0.0f;
}

void DSP_Process(DSP_Engine_t *engine, float *temp, float *press, float *alt, float dt) {
    if (temp != NULL) {
        *temp = Cascade_Update(&engine->temp_filter, *temp, dt, &engine->temp_roc);
    }
    
    if (press != NULL) {
        *press = Cascade_Update(&engine->press_filter, *press, dt, &engine->press_roc);
    }
    
    if (alt != NULL) {
        *alt = Cascade_Update(&engine->alt_filter, *alt, dt, &engine->vertical_speed);
    }
}
