#include "rtos/event.h"
#include "rtos/task.h"

void EventGroup_Init(EventGroup_t *eg) {
    eg->flags = 0;
}

void EventGroup_SetBits(EventGroup_t *eg, uint32_t bits) {
    __asm volatile("cpsid i");
    eg->flags |= bits;
    // Unblock tasks waiting on these flags if any
    __asm volatile("cpsie i");
}

uint32_t EventGroup_WaitBits(EventGroup_t *eg, uint32_t bits, uint8_t wait_all, uint8_t clear_on_exit) {
    uint32_t return_flags = 0;
    while(1) {
        __asm volatile("cpsid i");
        uint8_t condition_met = 0;
        
        if (wait_all) {
            if ((eg->flags & bits) == bits) condition_met = 1;
        } else {
            if ((eg->flags & bits) != 0) condition_met = 1;
        }
        
        if (condition_met) {
            return_flags = eg->flags & bits;
            if (clear_on_exit) {
                eg->flags &= ~bits;
            }
            __asm volatile("cpsie i");
            break;
        }
        
        __asm volatile("cpsie i");
        Task_Yield();
    }
    return return_flags;
}
