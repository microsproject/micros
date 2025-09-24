/*--------------------------------------------------------------------------------------------------------------------*/
/* SPDX-License-Identifier: MIT */
/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @file main.c
 * @brief Example of using the MicrOS initialization framework
 */
/*--------------------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include "micros/init.h"
#include "micros/kernel.h"
/*--------------------------------------------------------------------------------------------------------------------*/
void clock_init(void) {
    /* This runs first - critical clock setup */
    printf("--> System clock initialized - simulated\n");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void uart_init(void) {
    printf("--> UART initialized - simulated\n");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void diagnostics_init(void) {
    /* This runs last - can use other initialized components */
    printf("--> System initialized successfully\n");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void very_early_init_func(void) {
    printf("--> Very early init function in section .init.05\n");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void very_late_fini_func(void) {
    printf("--> Very late fini function in section .fini_array\n");
}
/*--------------------------------------------------------------------------------------------------------------------*/
MICROS_REGISTER_INIT_EARLY_FUNCTION(clock_init, 0);
MICROS_REGISTER_INIT_FUNCTION(uart_init, 1);
MICROS_REGISTER_INIT_FUNCTION(diagnostics_init, 5);
MICROS_REGISTER_INIT_FUNCTION(very_early_init_func, 55);
MICROS_REGISTER_FINISH_FUNCTION(very_late_fini_func, 0);
/*--------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    printf("[main] MicrOS Initialization Framework Example\n");
    printf("[main] System is up and running!\n");
    k_delay_ms(1000);
    printf("[main] Main function is exiting now.\n");
    return 0;
}