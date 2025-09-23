/*--------------------------------------------------------------------------------------------------------------------*/
/* SPDX-License-Identifier: MIT */
/*--------------------------------------------------------------------------------------------------------------------*/
#include <stddef.h>
#include <stdlib.h>
#include "ARMCM3.h"
#include "core_cm3.h"
#include "micros/kernel.h"
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct micros_thread micros_thread_t;
typedef struct micros_thread {
    uint32_t* sp;
    uint32_t* stack;
    micros_thread_t* next;
} micros_thread_t;
/*--------------------------------------------------------------------------------------------------------------------*/
static volatile int scheduler_lock_counter = 0;
static micros_thread_t* threads = NULL;
static volatile micros_thread_t* current_thread = NULL;
static volatile bool scheduler_enabled = false;
/*--------------------------------------------------------------------------------------------------------------------*/
// uint32_t* micros_context_switch(uint32_t* old_sp) {
//     current_thread->sp = old_sp;
//     if (current_thread->next != NULL) {
//         current_thread = current_thread->next;
//     } else {
//         current_thread = threads;
//     }
//     return current_thread->sp;
// }
// /*--------------------------------------------------------------------------------------------------------------------*/
__attribute__((naked)) void PendSV_Handler(void) {
    if (!scheduler_enabled) {
        __asm volatile(
            "BX LR                       \n"  // Return from exception.
        );
    }
    if (__get_PSP() == 0) {
        // first context switch, no current thread yet
        current_thread = threads;
        __set_PSP((uint32_t)(current_thread->sp) + 8 * sizeof(uint32_t));
        __asm volatile(
            "ORR LR,LR, 0x04            \n"   // Ensure exception
                                              // return uses PSP.
            "BX LR                       \n"  // Return from exception.
        );
    }
    __asm volatile(
        "MRS R0, PSP            \n"  // Get current PSP value into R0
        "STMDB R0!, {R4-R11}    \n"  // Store R4-R11 onto the stack,
                                     // decrement PSP
        "MSR PSP, R0            \n"  // Update PSP
    );
    current_thread->sp = (uint32_t*)__get_PSP();
    if (current_thread->next != NULL) {
        current_thread = current_thread->next;
    } else {
        current_thread = threads;
    }
    __asm volatile(
        "LDMIA %0!, {R4-R11}    \n"       // Pop R4-R11 from new thread's stack
        "MSR PSP, %0            \n"       // Update PSP with new value
        "ORR LR,LR, 0x04            \n"   // Ensure exception return uses
        "BX LR                       \n"  // Return from exception.
        :
        : "r"(current_thread->sp)
        : "memory");
}
/*--------------------------------------------------------------------------------------------------------------------*/
uint32_t k_enter_critical(void) {
    uint32_t state = __get_PRIMASK();
    __disable_irq();
    return state;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void k_leave_critical(uint32_t state) {
    __set_PRIMASK(state);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void k_scheduler_lock(void) {
    uint32_t state = k_enter_critical();
    scheduler_lock_counter++;
    k_leave_critical(state);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void k_scheduler_unlock(void) {
    uint32_t state = k_enter_critical();
    if (scheduler_lock_counter > 0) {
        scheduler_lock_counter--;
    }
    k_leave_critical(state);
}
/*--------------------------------------------------------------------------------------------------------------------*/
bool k_scheduler_is_locked(void) {
    return scheduler_lock_counter > 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int k_task_create(void (*task_func)(void*),
                  void* arg,
                  uint32_t* stack_base,
                  size_t stack_size) {
    if (task_func == NULL || stack_base == NULL || stack_size < 32) {
        return -1;
    }

    micros_thread_t* new_thread =
        (micros_thread_t*)calloc(1, sizeof(micros_thread_t));
    if (new_thread == NULL) {
        return -2;
    }

    uint32_t* stack_top = stack_base + (stack_size / sizeof(uint32_t));
    new_thread->stack = stack_top;
    new_thread->sp = stack_top;

    /* Exception stack frame (hardware-saved) */
    *(--new_thread->sp) = 0x01000000;           // xPSR (Thumb bit set)
    *(--new_thread->sp) = (uint32_t)task_func;  // PC = entry point
    *(--new_thread->sp) = 0xFFFFFFFD;     // LR = return with PSP to Thread mode
    *(--new_thread->sp) = 0x12121212;     // R12
    *(--new_thread->sp) = 0x03030303;     // R3
    *(--new_thread->sp) = 0x02020202;     // R2
    *(--new_thread->sp) = 0x01010101;     // R1
    *(--new_thread->sp) = (uint32_t)arg;  // R0

    /* Software-saved context (R4-R11) */
    for (int i = 0; i < 8; i++) {
        *(--new_thread->sp) = 0xDEADBEEF;
    }

    uint32_t state = k_enter_critical();
    new_thread->next = threads;
    threads = new_thread;
    k_leave_critical(state);

    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void k_task_yield(void) {
    uint32_t state = k_enter_critical();

    if (scheduler_lock_counter == 0 && scheduler_enabled) {
        SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
    }
    k_leave_critical(state);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void k_scheduler_start(void) {
    if (threads == NULL) {
        return;
    }

    uint32_t state = k_enter_critical();

    if (!scheduler_enabled) {
        scheduler_enabled = true;
    }
    __set_PRIMASK(state);

    while (1) {
        k_delay_ms(1000);
    }
}