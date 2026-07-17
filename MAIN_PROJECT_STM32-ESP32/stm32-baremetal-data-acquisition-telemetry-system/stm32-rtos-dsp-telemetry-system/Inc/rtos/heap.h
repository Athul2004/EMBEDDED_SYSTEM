#ifndef RTOS_HEAP_H
#define RTOS_HEAP_H
#include <stddef.h>

void Heap_Init(void);
void* Heap_Malloc(size_t size);
void Heap_Free(void *ptr);

#endif // RTOS_HEAP_H
