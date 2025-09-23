/* SPDX-License-Identifier: MIT */
/*--------------------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
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
extern uint32_t __ctors_start__;
extern uint32_t __ctors_end__;
extern uint32_t __dtors_start__;
extern uint32_t __dtors_end__;
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
void Reset_Handler(void) {
    uint32_t* src = &__data_load__;
    uint32_t* dst = &__data_start__;
    while (dst < &__data_end__) {
        *dst++ = *src++;
    }

    dst = &__bss_start__;
    while (dst < &__bss_end__) {
        *dst++ = 0;
    }

    SystemInit();
    NVIC_SetPriority(PendSV_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); // Lowest priority

    // for (void (**ctor)(void) = (void (**)(void))&__ctors_start__;
    //      ctor < (void (**)(void))&__ctors_end__; ctor++) {
    //     if (*ctor)
    //         (**ctor)();
    // }

    (void)main();

    // for (void (**dtor)(void) = (void (**)(void))&__dtors_start__;
    //      dtor < (void (**)(void))&__dtors_end__; dtor++) {
    //     if (*dtor)
    //         (**dtor)();
    // }

    while (1) {
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
