/* SPDX-License-Identifier: MIT */
/*--------------------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include "ARMCM3.h"
#include "core_cm3.h"
#include "micros/kernel.h"
/*--------------------------------------------------------------------------------------------------------------------*/
extern uint32_t __stack_top__;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __data_load__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t __init_start__;
extern uint32_t __init_end__;
static volatile uint32_t tick_counter = 0;

/*--------------------------------------------------------------------------------------------------------------------*/
extern int main(void);
extern void PendSV_Handler(void);
/*--------------------------------------------------------------------------------------------------------------------*/
void Reset_Handler(void);
/*--------------------------------------------------------------------------------------------------------------------*/
__attribute__((weak)) void SystemInit(void) {}
/*--------------------------------------------------------------------------------------------------------------------*/
void SysTick_Handler(void) {
    tick_counter++;
    if ((tick_counter % 10) == 0) {
        k_task_yield();
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
__attribute__((weak)) void Default_Handler(void) {
    while (1) {
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
__attribute__((weak, section(".init"), used)) void Default_HandlerReturn(void) {
    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/
__attribute__((weak,
               alias("Default_HandlerReturn"),
               section(".init"),
               used)) void
_init(void);
__attribute__((weak, alias("Default_Handler"))) void NMI_Handler(void);
__attribute__((weak, alias("Default_Handler"))) void HardFault_Handler(void);
__attribute__((weak, alias("Default_Handler"))) void MemManage_Handler(void);
__attribute__((weak, alias("Default_Handler"))) void BusFault_Handler(void);
__attribute__((weak, alias("Default_Handler"))) void UsageFault_Handler(void);
__attribute__((weak, alias("Default_Handler"))) void SVC_Handler(void);
__attribute__((weak, alias("Default_Handler"))) void DebugMon_Handler(void);
/*--------------------------------------------------------------------------------------------------------------------*/
__attribute__((section(".isr_vector"))) const void* vector_table[240] = {
    (void*)&__stack_top__,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0,
    0,
    0,
    0, /* reserved */
    SVC_Handler,
    DebugMon_Handler,
    0, /* reserved */
    PendSV_Handler,
    SysTick_Handler,
    /* add IRQ handlers below as needed */
};
/*--------------------------------------------------------------------------------------------------------------------*/
static void copy_data(void) {
    uint32_t* src = &__data_load__;
    uint32_t* dst = &__data_start__;
    while (dst < &__data_end__) {
        *dst++ = *src++;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void zero_bss(void) {
    uint32_t* dst = &__bss_start__;
    while (dst < &__bss_end__) {
        *dst++ = 0;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void run_init_functions(void) {
    /* The .init section contains actual function code, not function pointers.
     * We need to use a different approach - either use init_array or
     * create a custom function pointer table. For now, let's use a simple
     * approach where init functions register themselves. */

    /* Alternative approach: Use constructor attribute for automatic
     * registration */
    extern void (*__preinit_array_start[])(void);
    extern void (*__preinit_array_end[])(void);
    extern void (*__init_array_start[])(void);
    extern void (*__init_array_end[])(void);

    for (void (**preinit_func)(void) = __preinit_array_start;
         preinit_func < __preinit_array_end; preinit_func++) {
        if (*preinit_func) {
            (*preinit_func)();
        }
    }

    for (void (**init_func)(void) = __init_array_start;
         init_func < __init_array_end; init_func++) {
        if (*init_func) {
            (*init_func)();
        }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void run_fini_functions(void) {
    extern void (*__fini_array_start[])(void);
    extern void (*__fini_array_end[])(void);

    for (void (**fini_func)(void) = __fini_array_start;
         fini_func < __fini_array_end; fini_func++) {
        if (*fini_func) {
            (*fini_func)();
        }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void Reset_Handler(void) {
    copy_data();
    zero_bss();

    SystemInit();
    NVIC_SetPriority(PendSV_IRQn,
                     (1UL << __NVIC_PRIO_BITS) - 1UL);  // Lowest priority

    run_init_functions();
    int return_code = main();
    printf("[boot] Main function returned with code %d\n", return_code);
    run_fini_functions();

    printf("[boot] System halted after main return\n");
    while (1) {
        __WFI();
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void k_delay_ms(uint32_t ms) {
    uint32_t target = tick_counter + ms;
    while (tick_counter < target) {
        __WFI();
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
