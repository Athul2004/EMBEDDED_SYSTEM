#ifndef FILTER_MEDIAN_H
#define FILTER_MEDIAN_H

#define MEDIAN_WINDOW_SIZE 5

typedef struct {
    float buffer[MEDIAN_WINDOW_SIZE];
    int index;
} Median_Filter_t;

void Median_Init(Median_Filter_t *filter, float initial_value);
float Median_Update(Median_Filter_t *filter, float new_value);

#endif // FILTER_MEDIAN_H
