#include "rtos/timer.h"
#include <stddef.h>

#define MAX_TIMERS 10
static Timer_t *active_timers[MAX_TIMERS];
static int timer_count = 0;

void Timer_Init(Timer_t *timer, uint32_t period_ticks, uint8_t auto_reload, TimerCallback_t callback, void *arg) {
    timer->period_ticks = period_ticks;
    timer->remaining_ticks = period_ticks;
    timer->auto_reload = auto_reload;
    timer->active = 0;
    timer->callback = callback;
    timer->callback_arg = arg;
}

void Timer_Start(Timer_t *timer) {
    __asm volatile("cpsid i");
    if (!timer->active && timer_count < MAX_TIMERS) {
        active_timers[timer_count++] = timer;
        timer->active = 1;
        timer->remaining_ticks = timer->period_ticks;
    }
    __asm volatile("cpsie i");
}

void Timer_Stop(Timer_t *timer) {
    __asm volatile("cpsid i");
    if (timer->active) {
        for (int i = 0; i < timer_count; i++) {
            if (active_timers[i] == timer) {
                active_timers[i] = active_timers[--timer_count];
                timer->active = 0;
                break;
            }
        }
    }
    __asm volatile("cpsie i");
}

void Timer_SystemTickUpdate(void) {
    for (int i = 0; i < timer_count; i++) {
        if (active_timers[i]->remaining_ticks > 0) {
            active_timers[i]->remaining_ticks--;
            if (active_timers[i]->remaining_ticks == 0) {
                if (active_timers[i]->callback) {
                    active_timers[i]->callback(active_timers[i]->callback_arg);
                }
                if (active_timers[i]->auto_reload) {
                    active_timers[i]->remaining_ticks = active_timers[i]->period_ticks;
                } else {
                    Timer_Stop(active_timers[i]);
                    i--; // Adjust index
                }
            }
        }
    }
}
