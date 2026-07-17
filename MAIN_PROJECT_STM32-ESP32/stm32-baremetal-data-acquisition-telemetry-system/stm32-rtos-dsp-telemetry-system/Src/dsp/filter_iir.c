#include "dsp/filter_iir.h"

void IIR_Init(IIR_Filter_t *filter, float alpha, float initial_value) {
    if (alpha <= 0.0f) alpha = 0.01f;
    if (alpha > 1.0f) alpha = 1.0f;
    
    filter->alpha = alpha;
    filter->last_value = initial_value;
}

float IIR_Update(IIR_Filter_t *filter, float new_value) {
    // y[n] = alpha * x[n] + (1 - alpha) * y[n-1]
    filter->last_value = (filter->alpha * new_value) + ((1.0f - filter->alpha) * filter->last_value);
    return filter->last_value;
}
