#ifndef FILTER_KALMAN_H
#define FILTER_KALMAN_H

typedef struct {
    // State Vector [Altitude, Velocity]
    float x_alt;
    float x_vel;
    
    // Covariance Matrix P
    float p00, p01, p10, p11;
    
    // Process Noise Covariance Q
    float q_alt;
    float q_vel;
    
    // Measurement Noise Covariance R
    float r_measure;
    
} Kalman2D_Filter_t;

void Kalman2D_Init(Kalman2D_Filter_t *filter, float r_measure, float q_alt, float q_vel, float initial_alt);
void Kalman2D_Update(Kalman2D_Filter_t *filter, float altitude_measurement, float dt);

#endif // FILTER_KALMAN_H
