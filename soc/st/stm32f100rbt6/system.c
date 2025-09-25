/*--------------------------------------------------------------------------------------------------------------------*/
/* SPDX-License-Identifier: MIT */
/*--------------------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
/*--------------------------------------------------------------------------------------------------------------------*/
#define PERIPH_BASE 0x40000000UL
#define APB2PERIPH_BASE (PERIPH_BASE + 0x10000UL)
/*--------------------------------------------------------------------------------------------------------------------*/
#define USART1_BASE (APB2PERIPH_BASE + 0x3800UL)

#define USART1_SR (*(volatile uint32_t*)(USART1_BASE + 0x00))
#define USART1_DR (*(volatile uint32_t*)(USART1_BASE + 0x04))
#define USART1_BRR (*(volatile uint32_t*)(USART1_BASE + 0x08))
#define USART1_CR1 (*(volatile uint32_t*)(USART1_BASE + 0x0C))

#define USART_CR1_UE (1U << 13)
#define USART_CR1_TE (1U << 3)
#define USART_SR_TXE (1U << 7)
/*--------------------------------------------------------------------------------------------------------------------*/
#define RCC_BASE 0x40021000UL
#define RCC_CR (*(volatile uint32_t*)(RCC_BASE + 0x00))
#define RCC_CFGR (*(volatile uint32_t*)(RCC_BASE + 0x04))

#define RCC_CR_PLLON (1U << 24)
#define RCC_CR_PLLRDY (1U << 25)
#define RCC_CFGR_SW_PLL (0x2U << 0)
#define RCC_CFGR_HPRE_DIV1 (0x0U << 4)
#define RCC_CFGR_PPRE_DIV1 (0x0U << 8)

#define PCLK2_HZ 24000000U
#define UART1_BAUD 115200U
/*--------------------------------------------------------------------------------------------------------------------*/
void SystemInit(void) {
    // RCC_CFGR |= RCC_CFGR_SW_PLL;
    // RCC_CR |= RCC_CR_PLLON;
    // while (!(RCC_CR & RCC_CR_PLLRDY))
    //     ;
    // RCC_CFGR |= RCC_CFGR_HPRE_DIV1;
    // RCC_CFGR |= RCC_CFGR_PPRE_DIV1;

    uint32_t baud = UART1_BAUD;
    uint32_t div = (PCLK2_HZ + (baud / 2U)) / baud; /* rounded */

    USART1_BRR = div;
    USART1_CR1 = USART_CR1_UE | USART_CR1_TE; /* enable, TX only */
}
/*--------------------------------------------------------------------------------------------------------------------*/
void stdout_putc(char c) {
    while (!(USART1_SR & USART_SR_TXE)) {
        /* wait until transmit buffer empty */
    }
    USART1_DR = (uint32_t)(uint8_t)c;
}
/*--------------------------------------------------------------------------------------------------------------------*/
