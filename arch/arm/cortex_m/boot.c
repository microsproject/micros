/*--------------------------------------------------------------------------------------------------------------------*/
/* SPDX-License-Identifier: MIT */
/*--------------------------------------------------------------------------------------------------------------------*/
#define CONFIG_MICROS_LOG_ENABLE
#define CONFIG_MICROS_LOG_LEVEL MICROS_LOG_LEVEL_DEBUG
/*--------------------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include "ARMCM3.h"
#include "core_cm3.h"
#include "micros/kernel.h"
#include "micros/log.h"
/*--------------------------------------------------------------------------------------------------------------------*/
extern uint32_t __stack_top__;
/*--------------------------------------------------------------------------------------------------------------------*/
MICROS_LOG_REGISTER(boot, MICROS_LOG_LEVEL_INFO);
/*--------------------------------------------------------------------------------------------------------------------*/
extern int main(void);
void Reset_Handler(void);
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
void NMI_Handler(void);
void HardFault_Handler(void);
/*--------------------------------------------------------------------------------------------------------------------*/
static void copy_data(void) {
    extern uint32_t __data_start__;
    extern uint32_t __data_end__;
    extern uint32_t __data_load__;

    uint32_t* src = &__data_load__;
    uint32_t* dst = &__data_start__;
    while (dst < &__data_end__) {
        *dst++ = *src++;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void zero_bss(void) {
    extern uint32_t __bss_start__;
    extern uint32_t __bss_end__;

    uint32_t* dst = &__bss_start__;
    while (dst < &__bss_end__) {
        *dst++ = 0;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void run_init_functions(void) {
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

    run_init_functions();
    int return_code = main();
    I("Main function returned with code %d", return_code);
    run_fini_functions();

    I("System halted after main return");
    while (1) {
        __WFI();
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
