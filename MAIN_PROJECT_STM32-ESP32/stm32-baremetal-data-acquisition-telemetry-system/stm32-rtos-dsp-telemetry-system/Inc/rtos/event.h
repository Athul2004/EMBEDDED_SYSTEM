#ifndef RTOS_EVENT_H
#define RTOS_EVENT_H
#include <stdint.h>

typedef struct {
    uint32_t flags;
} EventGroup_t;

void EventGroup_Init(EventGroup_t *eg);
void EventGroup_SetBits(EventGroup_t *eg, uint32_t bits);
uint32_t EventGroup_WaitBits(EventGroup_t *eg, uint32_t bits, uint8_t wait_all, uint8_t clear_on_exit);

#endif // RTOS_EVENT_H
