    .syntax unified
    .cpu cortex-m4
    .thumb

    .global PendSV_Handler
    .global current_tcb
    .global next_tcb
    .extern Task_SelectNext

    .section .text
    .type PendSV_Handler, %function
PendSV_Handler:
    // Disable interrupts
    CPSID I

    // Get current process stack pointer (PSP)
    MRS R0, PSP

    // Save R4-R11 and EXC_RETURN (LR) on process stack
    STMDB R0!, {R4-R11, LR}

    // Save current SP to current_tcb->sp
    LDR R1, =current_tcb
    LDR R2, [R1]      // R2 = current_tcb
    STR R0, [R2]      // current_tcb->sp = R0

    // Call Task_SelectNext to update current_tcb and next_tcb
    PUSH {LR}
    BL Task_SelectNext
    POP {LR}

    // current_tcb = next_tcb
    LDR R1, =current_tcb
    LDR R2, =next_tcb
    LDR R3, [R2]      // R3 = next_tcb
    STR R3, [R1]      // current_tcb = next_tcb

    // Load new task's SP
    LDR R0, [R3]      // R0 = new_tcb->sp

    // Restore R4-R11 and EXC_RETURN (LR) from new process stack
    LDMIA R0!, {R4-R11, LR}

    // Set PSP to new process stack pointer
    MSR PSP, R0

    // Enable interrupts
    CPSIE I

    // Return from exception
    BX LR
