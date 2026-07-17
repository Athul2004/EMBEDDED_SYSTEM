#include "dsp/filter_median.h"



void Median_Init(Median_Filter_t *filter, float initial_value) {
    for (int i = 0; i < MEDIAN_WINDOW_SIZE; i++) {
        filter->buffer[i] = initial_value;
    }
    filter->index = 0;
}

float Median_Update(Median_Filter_t *filter, float new_value) {
    // Insert new value into circular buffer
    filter->buffer[filter->index] = new_value;
    filter->index = (filter->index + 1) % MEDIAN_WINDOW_SIZE;
    
    // Copy buffer for sorting (so we don't disrupt the circular order)
    float sorted[MEDIAN_WINDOW_SIZE];
    for (int i = 0; i < MEDIAN_WINDOW_SIZE; i++) {
        sorted[i] = filter->buffer[i];
    }
    
    // Simple insertion sort (efficient for small N like 5)
    for (int i = 1; i < MEDIAN_WINDOW_SIZE; i++) {
        float key = sorted[i];
        int j = i - 1;
        while (j >= 0 && sorted[j] > key) {
            sorted[j + 1] = sorted[j];
            j = j - 1;
        }
        sorted[j + 1] = key;
    }
    
    // Return the middle element
    return sorted[MEDIAN_WINDOW_SIZE / 2];
}
