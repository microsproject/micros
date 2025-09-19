/* SPDX-License-Identifier: MIT */
/*--------------------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
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
/*--------------------------------------------------------------------------------------------------------------------*/
extern int main(void);
/*--------------------------------------------------------------------------------------------------------------------*/
void Reset_Handler(void);
/*--------------------------------------------------------------------------------------------------------------------*/
__attribute__((weak)) void SystemInit(void) {}
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
__attribute__((weak, alias("Default_Handler"))) void PendSV_Handler(void);
__attribute__((weak, alias("Default_Handler"))) void SysTick_Handler(void);
/*--------------------------------------------------------------------------------------------------------------------*/
__attribute__((section(".isr_vector"))) const void* vector_table[] = {
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
