/* SPDX-License-Identifier: MIT */
/*--------------------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include "ARMCM3.h"
#include "core_cm3.h"
#include "micros/kernel.h"
#define SYSTEM_CORE_CLOCK 50000000U
/*--------------------------------------------------------------------------------------------------------------------*/
#define UART0_DR (*(volatile uint32_t*)0x4000C000)
#define UART0_FR (*(volatile uint32_t*)0x4000C018)
#define UART_FR_TXFF (1 << 5)
/*--------------------------------------------------------------------------------------------------------------------*/
void SystemInit(void) {
    SysTick->LOAD = (SYSTEM_CORE_CLOCK / 1000 / 2) - 1; /* 1ms */
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;

    NVIC_SetPriority(SysTick_IRQn, 1UL);
    NVIC_EnableIRQ(SysTick_IRQn);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void stdout_putc(char c) {
    while (UART0_FR & UART_FR_TXFF)
        ;
    UART0_DR = c;
}
/*--------------------------------------------------------------------------------------------------------------------*/
