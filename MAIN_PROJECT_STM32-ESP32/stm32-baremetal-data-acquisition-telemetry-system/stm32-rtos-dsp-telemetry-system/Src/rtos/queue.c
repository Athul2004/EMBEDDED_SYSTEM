#include "rtos/queue.h"
#include "rtos/task.h"
#include <string.h>

void Queue_Init(Queue_t *q, void *buffer, size_t item_size, size_t capacity) {
    q->buffer = buffer;
    q->item_size = item_size;
    q->capacity = capacity;
    q->count = 0;
    q->head = 0;
    q->tail = 0;
}

int Queue_Send(Queue_t *q, const void *item) {
    __asm volatile("cpsid i");
    while (q->count == q->capacity) {
        __asm volatile("cpsie i");
        Task_Yield();
        __asm volatile("cpsid i");
    }
    
    char *dest = (char *)q->buffer + (q->tail * q->item_size);
    memcpy(dest, item, q->item_size);
    q->tail = (q->tail + 1) % q->capacity;
    q->count++;
    
    __asm volatile("cpsie i");
    return 0; // Success
}

int Queue_Receive(Queue_t *q, void *item) {
    __asm volatile("cpsid i");
    while (q->count == 0) {
        __asm volatile("cpsie i");
        Task_Yield();
        __asm volatile("cpsid i");
    }
    
    char *src = (char *)q->buffer + (q->head * q->item_size);
    memcpy(item, src, q->item_size);
    q->head = (q->head + 1) % q->capacity;
    q->count--;
    
    __asm volatile("cpsie i");
    return 0; // Success
}
