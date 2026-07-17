#include "dsp/filter_cascade.h"
#include <stddef.h>

void Cascade_Init(Cascade_Filter_t *filter, float initial_value, float iir_alpha, float kalman_r, float kalman_q_val, float kalman_q_vel) {
    Median_Init(&filter->median, initial_value);
    IIR_Init(&filter->iir, iir_alpha, initial_value);
    Kalman2D_Init(&filter->kalman, kalman_r, kalman_q_val, kalman_q_vel, initial_value);
}

float Cascade_Update(Cascade_Filter_t *filter, float new_value, float dt, float *velocity_out) {
    // Stage 1: Median Filter to reject anomalies
    float median_out = Median_Update(&filter->median, new_value);
    
    // Stage 2: IIR Filter to smooth out high-frequency noise
    float iir_out = IIR_Update(&filter->iir, median_out);
    
    // Stage 3: Kinematic Kalman Filter for State and Velocity tracking
    Kalman2D_Update(&filter->kalman, iir_out, dt);
    
    // Optional: Return the calculated velocity (rate of change)
    if (velocity_out != NULL) {
        *velocity_out = filter->kalman.x_vel;
    }
    
    // Return the primary filtered state
    return filter->kalman.x_alt;
}
