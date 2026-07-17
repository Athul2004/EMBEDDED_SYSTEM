#ifndef RTOS_QUEUE_H
#define RTOS_QUEUE_H
#include <stdint.h>
#include <stddef.h>

typedef struct {
    void *buffer;
    size_t item_size;
    size_t capacity;
    size_t count;
    size_t head;
    size_t tail;
} Queue_t;

void Queue_Init(Queue_t *q, void *buffer, size_t item_size, size_t capacity);
int Queue_Send(Queue_t *q, const void *item);
int Queue_Receive(Queue_t *q, void *item);

#endif // RTOS_QUEUE_H
