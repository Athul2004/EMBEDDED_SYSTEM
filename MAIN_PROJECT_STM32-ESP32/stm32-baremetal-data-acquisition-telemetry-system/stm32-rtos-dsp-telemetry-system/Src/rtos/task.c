#include "rtos/task.h"
#include <stddef.h>

TCB_t tcb_array[MAX_TASKS];
TCB_t *current_tcb = NULL;
TCB_t *next_tcb = NULL;
static int task_count = 0;
static int current_task_idx = 0;

#define SCB_ICSR        (*((volatile uint32_t *)0xE000ED04))
#define SCB_ICSR_PENDSVSET (1 << 28)

void Task_InitSystem(void) {
    task_count = 0;
    current_task_idx = 0;
    current_tcb = NULL;
    next_tcb = NULL;
}

int Task_Create(void (*task_func)(void), uint32_t priority) {
    if (task_count >= MAX_TASKS) return -1;

    TCB_t *tcb = &tcb_array[task_count];
    
    // Initialize stack for Cortex-M
    uint32_t *sp = &tcb->stack[STACK_SIZE]; // Top of stack
    
    // Stack frame for exception return (xPSR, PC, LR, R12, R3, R2, R1, R0)
    *(--sp) = 0x01000000; // xPSR (Thumb bit set)
    *(--sp) = (uint32_t)task_func; // PC
    *(--sp) = 0xFFFFFFFD; // LR (Return to Thread mode using PSP)
    *(--sp) = 0; // R12
    *(--sp) = 0; // R3
    *(--sp) = 0; // R2
    *(--sp) = 0; // R1
    *(--sp) = 0; // R0

    // EXC_RETURN to be popped by PendSV
    *(--sp) = 0xFFFFFFFD;

    // Additional registers saved by context switch (R11-R4)
    for (int i = 0; i < 8; i++) {
        *(--sp) = 0;
    }

    tcb->sp = sp;
    tcb->state = TASK_STATE_READY;
    tcb->priority = priority;
    tcb->delay_ticks = 0;

    task_count++;
    return task_count - 1;
}

void Task_StartFirst(void) {
    if (task_count == 0) return;
    
    current_tcb = &tcb_array[0];
    current_tcb->state = TASK_STATE_RUNNING;
    
    // Set PSP to the top of the stack of the first task
    __asm volatile (
        "LDR R0, =current_tcb \n"
        "LDR R1, [R0] \n"      // R1 = current_tcb
        "LDR R2, [R1] \n"      // R2 = current_tcb->sp
        "MSR PSP, R2 \n"       // Set PSP
        "MOVS R0, #2 \n"       // Switch to PSP
        "MSR CONTROL, R0 \n"
        "ISB \n"
        "POP {R4-R11} \n"      // Pop R4-R11 manually for the first task
        "POP {R0} \n"          // Pop EXC_RETURN (discard)
        "POP {R0-R3} \n"       // Pop R0-R3
        "POP {R12} \n"
        "POP {LR} \n"          // Pop LR
        "POP {LR} \n"          // Pop PC to LR to jump
        "POP {R1} \n"          // Pop xPSR
        "BX LR \n"             // Jump to task
    );
}

void Task_SelectNext(void) {
    // Simple round robin among ready tasks
    int start_idx = current_task_idx;
    do {
        current_task_idx = (current_task_idx + 1) % task_count;
        if (tcb_array[current_task_idx].state == TASK_STATE_READY) {
            break;
        }
    } while (current_task_idx != start_idx);
    
    next_tcb = &tcb_array[current_task_idx];
    if(current_tcb->state == TASK_STATE_RUNNING) {
        current_tcb->state = TASK_STATE_READY;
    }
    next_tcb->state = TASK_STATE_RUNNING;
}

void Task_UpdateDelays(void) {
    for (int i = 0; i < task_count; i++) {
        if (tcb_array[i].state == TASK_STATE_BLOCKED && tcb_array[i].delay_ticks > 0) {
            tcb_array[i].delay_ticks--;
            if (tcb_array[i].delay_ticks == 0) {
                tcb_array[i].state = TASK_STATE_READY;
            }
        }
    }
}

void Task_Yield(void) {
    SCB_ICSR |= SCB_ICSR_PENDSVSET;
}

void Task_Delay(uint32_t ticks) {
    current_tcb->delay_ticks = ticks;
    current_tcb->state = TASK_STATE_BLOCKED;
    Task_Yield();
}
