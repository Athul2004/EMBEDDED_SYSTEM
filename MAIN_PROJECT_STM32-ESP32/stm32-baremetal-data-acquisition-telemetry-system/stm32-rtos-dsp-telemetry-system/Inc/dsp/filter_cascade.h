#ifndef FILTER_CASCADE_H
#define FILTER_CASCADE_H

#include "filter_median.h"
#include "filter_iir.h"
#include "filter_kalman.h"

typedef struct {
    Median_Filter_t median;
    IIR_Filter_t iir;
    Kalman2D_Filter_t kalman;
} Cascade_Filter_t;

void Cascade_Init(Cascade_Filter_t *filter, float initial_value, float iir_alpha, float kalman_r, float kalman_q_val, float kalman_q_vel);
float Cascade_Update(Cascade_Filter_t *filter, float new_value, float dt, float *velocity_out);

#endif // FILTER_CASCADE_H
