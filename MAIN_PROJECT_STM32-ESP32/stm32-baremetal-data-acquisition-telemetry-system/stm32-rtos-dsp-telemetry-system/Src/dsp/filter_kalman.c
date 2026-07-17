#include "dsp/filter_kalman.h"

void Kalman2D_Init(Kalman2D_Filter_t *filter, float r_measure, float q_alt, float q_vel, float initial_alt) {
    filter->x_alt = initial_alt;
    filter->x_vel = 0.0f;
    
    filter->p00 = 1.0f;
    filter->p01 = 0.0f;
    filter->p10 = 0.0f;
    filter->p11 = 1.0f;
    
    filter->q_alt = q_alt;
    filter->q_vel = q_vel;
    
    filter->r_measure = r_measure;
}

void Kalman2D_Update(Kalman2D_Filter_t *filter, float altitude_measurement, float dt) {
    // --- Step 1: Predict ---
    // State Prediction:
    // x_alt = x_alt + dt * x_vel
    // x_vel = x_vel
    filter->x_alt += dt * filter->x_vel;
    
    // Covariance Prediction:
    // P = F * P * F^T + Q
    // F = [1 dt]
    //     [0  1]
    float p00_temp = filter->p00 + dt * filter->p10 + dt * (filter->p01 + dt * filter->p11) + filter->q_alt;
    float p01_temp = filter->p01 + dt * filter->p11;
    float p10_temp = filter->p10 + dt * filter->p11;
    float p11_temp = filter->p11 + filter->q_vel;
    
    filter->p00 = p00_temp;
    filter->p01 = p01_temp;
    filter->p10 = p10_temp;
    filter->p11 = p11_temp;
    
    // --- Step 2: Update ---
    // Innovation (Measurement Residual)
    float y = altitude_measurement - filter->x_alt;
    
    // Innovation Covariance
    // S = H * P * H^T + R
    // H = [1 0]
    float S = filter->p00 + filter->r_measure;
    
    // Kalman Gain
    // K = P * H^T * S^-1
    float k0 = filter->p00 / S;
    float k1 = filter->p10 / S;
    
    // State Update
    filter->x_alt += k0 * y;
    filter->x_vel += k1 * y;
    
    // Covariance Update
    // P = (I - K * H) * P
    p00_temp = (1.0f - k0) * filter->p00;
    p01_temp = (1.0f - k0) * filter->p01;
    p10_temp = -k1 * filter->p00 + filter->p10;
    p11_temp = -k1 * filter->p01 + filter->p11;
    
    filter->p00 = p00_temp;
    filter->p01 = p01_temp;
    filter->p10 = p10_temp;
    filter->p11 = p11_temp;
}
