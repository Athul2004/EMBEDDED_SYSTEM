#ifndef RTOS_TIMER_H
#define RTOS_TIMER_H
#include <stdint.h>

typedef void (*TimerCallback_t)(void *arg);

typedef struct {
    uint32_t period_ticks;
    uint32_t remaining_ticks;
    uint8_t auto_reload;
    uint8_t active;
    TimerCallback_t callback;
    void *callback_arg;
} Timer_t;

void Timer_Init(Timer_t *timer, uint32_t period_ticks, uint8_t auto_reload, TimerCallback_t callback, void *arg);
void Timer_Start(Timer_t *timer);
void Timer_Stop(Timer_t *timer);
void Timer_SystemTickUpdate(void);

#endif // RTOS_TIMER_H
