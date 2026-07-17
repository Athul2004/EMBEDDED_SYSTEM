#ifndef FILTER_IIR_H
#define FILTER_IIR_H

typedef struct {
    float alpha; // Smoothing factor (0.0 < alpha <= 1.0)
    float last_value;
} IIR_Filter_t;

void IIR_Init(IIR_Filter_t *filter, float alpha, float initial_value);
float IIR_Update(IIR_Filter_t *filter, float new_value);

#endif // FILTER_IIR_H
