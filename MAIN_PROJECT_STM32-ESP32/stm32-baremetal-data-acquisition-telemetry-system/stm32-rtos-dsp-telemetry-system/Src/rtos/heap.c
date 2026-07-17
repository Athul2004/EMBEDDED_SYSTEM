#include "rtos/heap.h"
#include <stdint.h>

#define HEAP_SIZE 8192 // 8KB RTOS Heap
static uint8_t heap_buffer[HEAP_SIZE];
static size_t heap_allocated = 0;

void Heap_Init(void) {
    heap_allocated = 0;
}

void* Heap_Malloc(size_t size) {
    void *ptr = NULL;
    
    // Ensure 8-byte alignment
    size_t aligned_size = (size + 7) & ~7;
    
    __asm volatile("cpsid i");
    if (heap_allocated + aligned_size <= HEAP_SIZE) {
        ptr = &heap_buffer[heap_allocated];
        heap_allocated += aligned_size;
    }
    __asm volatile("cpsie i");
    
    return ptr;
}

void Heap_Free(void *ptr) {
    // Bump allocator logic doesn't support free
}
