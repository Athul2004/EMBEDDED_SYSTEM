#ifndef DSP_ENGINE_H
#define DSP_ENGINE_H

#include "filter_cascade.h"

typedef struct {
    Cascade_Filter_t temp_filter;
    Cascade_Filter_t press_filter;
    Cascade_Filter_t alt_filter;
    
    // Output velocities (rates of change)
    float temp_roc;
    float press_roc;
    float vertical_speed; // in m/s
    
} DSP_Engine_t;

// Pass in the expected loop interval dt (in seconds, e.g. 1.0f for 1 second)
void DSP_Init(DSP_Engine_t *engine);
void DSP_Process(DSP_Engine_t *engine, float *temp, float *press, float *alt, float dt);

#endif // DSP_ENGINE_H
