/*--------------------------------------------------------------------------------------------------------------------*/
/* SPDX-License-Identifier: MIT */
/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @file main.c
 * @brief Example of using the MicrOS initialization framework
 */
/*--------------------------------------------------------------------------------------------------------------------*/
#define CONFIG_MICROS_LOG_ENABLE
#define CONFIG_MICROS_LOG_LEVEL MICROS_LOG_LEVEL_DEBUG
/*--------------------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include "micros/init.h"
#include "micros/kernel.h"
#include "micros/log.h"
/*--------------------------------------------------------------------------------------------------------------------*/
MICROS_LOG_REGISTER(main, MICROS_LOG_LEVEL_DEBUG);
/*--------------------------------------------------------------------------------------------------------------------*/
void clock_init(void) {
    /* This runs first - critical clock setup */
    D("System clock initialized - simulated");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void uart_init(void) {
    D("UART initialized - simulated");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void diagnostics_init(void) {
    /* This runs last - can use other initialized components */
    D("System initialized successfully - simulated");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void very_early_init_func(void) {
    D("Very early init function in section .init_array.50 - simulated");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void very_late_fini_func(void) {
    D("Very late fini function in section .fini_array - simulated");
}
/*--------------------------------------------------------------------------------------------------------------------*/
MICROS_REGISTER_INIT_EARLY_FUNCTION(clock_init, 0);
MICROS_REGISTER_INIT_FUNCTION(uart_init, 1);
MICROS_REGISTER_INIT_FUNCTION(diagnostics_init, 5);
MICROS_REGISTER_INIT_FUNCTION(very_early_init_func, 55);
MICROS_REGISTER_FINISH_FUNCTION(very_late_fini_func, 0);
/*--------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    I("MicrOS Initialization Framework Example");
    I("System is up and running!");
    k_delay_ms(200);
    I("Main function is exiting now.");
    return 0;
}