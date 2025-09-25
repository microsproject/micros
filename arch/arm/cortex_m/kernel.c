/*--------------------------------------------------------------------------------------------------------------------*/
/* SPDX-License-Identifier: MIT */
/*--------------------------------------------------------------------------------------------------------------------*/
#define CONFIG_MICROS_LOG_ENABLE
#define CONFIG_MICROS_LOG_LEVEL MICROS_LOG_LEVEL_DEBUG
/*--------------------------------------------------------------------------------------------------------------------*/
#include "micros/kernel.h"
#include <stddef.h>
#include <stdlib.h>
#include "ARMCM3.h"
#include "core_cm3.h"
#include "micros/init.h"
#include "micros/log.h"
/*--------------------------------------------------------------------------------------------------------------------*/
MICROS_LOG_REGISTER(kernel, MICROS_LOG_LEVEL_INFO);
/*--------------------------------------------------------------------------------------------------------------------*/
#define CONFIG_CPU_CLOCK_HZ 50000000U
#define CONFIG_KERNEL_TICK_HZ 1000U
#define CONFIG_KERNEL_TICK_TO_YIELD 10U
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
static volatile uint32_t tick_counter = 0;
static volatile uint32_t yield_counter = 0;
/*--------------------------------------------------------------------------------------------------------------------*/
#define NUM_IRQS 46
#define NUM_SYSCALLS 32
#define SYSCALL_CUSTOM_NUMBER (2)
/*--------------------------------------------------------------------------------------------------------------------*/
static k_irq_handler_t irq_handlers[NUM_IRQS];
static k_syscall_handler_t custom_syscall_handlers[NUM_SYSCALLS];
/*--------------------------------------------------------------------------------------------------------------------*/
extern uint32_t __stack_top__;
extern void Reset_Handler(void);
/*--------------------------------------------------------------------------------------------------------------------*/
static void NMI_Handler(void);
static void HardFault_Handler(void);
static void MemManage_Handler(void);
static void BusFault_Handler(void);
static void UsageFault_Handler(void);
extern void PendSV_Handler(void);
static void SysTick_Handler(void);
static void SVC_Handler(void);
static void DebugMon_Handler(void);
static void Irq_Handler(void);
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
    SysTick_Handler,  // SysTick,
    Irq_Handler,      // Interrupt0_IRQn = 0,
    Irq_Handler,      // Interrupt1_IRQn = 1,
    Irq_Handler,      // Interrupt2_IRQn = 2,
    Irq_Handler,      // Interrupt3_IRQn = 3,
    Irq_Handler,      // Interrupt4_IRQn = 4,
    Irq_Handler,      // Interrupt5_IRQn = 5,
    Irq_Handler,      // Interrupt6_IRQn = 6,
    Irq_Handler,      // Interrupt7_IRQn = 7,
    Irq_Handler,      // Interrupt8_IRQn = 8,
    Irq_Handler,      // Interrupt9_IRQn = 9,
    Irq_Handler,      // Interrupt10_IRQn = 10,
    Irq_Handler,      // Interrupt11_IRQn = 11,
    Irq_Handler,      // Interrupt12_IRQn = 12,
    Irq_Handler,      // Interrupt13_IRQn = 13,
    Irq_Handler,      // Interrupt14_IRQn = 14,
    Irq_Handler,      // Interrupt15_IRQn = 15,
    Irq_Handler,      // Interrupt16_IRQn = 16,
    Irq_Handler,      // Interrupt17_IRQn = 17,
    Irq_Handler,      // Interrupt18_IRQn = 18,
    Irq_Handler,      // Interrupt19_IRQn = 19,
    Irq_Handler,      // Interrupt20_IRQn = 20,
    Irq_Handler,      // Interrupt21_IRQn = 21,
    Irq_Handler,      // Interrupt22_IRQn = 22,
    Irq_Handler,      // Interrupt23_IRQn = 23,
    Irq_Handler,      // Interrupt24_IRQn = 24,
    Irq_Handler,      // Interrupt25_IRQn = 25,
    Irq_Handler,      // Interrupt26_IRQn = 26,
    Irq_Handler,      // Interrupt27_IRQn = 27,
    Irq_Handler,      // Interrupt28_IRQn = 28,
    Irq_Handler,      // Interrupt29_IRQn = 29,
    Irq_Handler,      // Interrupt30_IRQn = 30,
    Irq_Handler,      // Interrupt31_IRQn = 31,
    Irq_Handler,      // Interrupt32_IRQn = 32,
    Irq_Handler,      // Interrupt33_IRQn = 33,
    Irq_Handler,      // Interrupt34_IRQn = 34,
    Irq_Handler,      // Interrupt35_IRQn = 35,
    Irq_Handler,      // Interrupt36_IRQn = 36,
    Irq_Handler,      // Interrupt37_IRQn = 37,
    Irq_Handler,      // Interrupt38_IRQn = 38,
    Irq_Handler,      // Interrupt39_IRQn = 39,
    Irq_Handler,      // Interrupt40_IRQn = 40,
    Irq_Handler,      // Interrupt41_IRQn = 41,
    Irq_Handler,      // Interrupt42_IRQn = 42,
    Irq_Handler,      // Interrupt43_IRQn = 43,
    Irq_Handler,      // Interrupt44_IRQn = 44,
    Irq_Handler,      // Interrupt45_IRQn = 45,
};
/*--------------------------------------------------------------------------------------------------------------------*/
static void NMI_Handler(void) {
    E("[NMI] A non-maskable interrupt has occurred!");
    while (1) {
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static __attribute__((used, naked)) void HardFault_HandlerStackFrame(
    uint32_t* stacked_regs) {
    // stacked_regs points to: r0, r1, r2, r3, r12, lr, pc, xPSR
    uint32_t r0 = stacked_regs[0];
    uint32_t r1 = stacked_regs[1];
    uint32_t r2 = stacked_regs[2];
    uint32_t r3 = stacked_regs[3];
    uint32_t r12 = stacked_regs[4];
    uint32_t lr = stacked_regs[5];   // Link register
    uint32_t pc = stacked_regs[6];   // Program counter
    uint32_t psr = stacked_regs[7];  // Program status register

    // System fault status registers
    uint32_t cfsr = SCB->CFSR;    // Configurable Fault Status
    uint32_t hfsr = SCB->HFSR;    // HardFault Status
    uint32_t dfsr = SCB->DFSR;    // Debug Fault Status
    uint32_t afsr = SCB->AFSR;    // Auxiliary Fault Status
    uint32_t mmfar = SCB->MMFAR;  // MemManage Fault Address
    uint32_t bfar = SCB->BFAR;    // Bus Fault Address

    E("!!!! HardFault !!!!");
    E("   R0 = 0x%08lX", r0);
    E("   R1 = 0x%08lX", r1);
    E("   R2 = 0x%08lX", r2);
    E("   R3 = 0x%08lX", r3);
    E("  R12 = 0x%08lX", r12);
    E("   LR = 0x%08lX", lr);
    E("   PC = 0x%08lX", pc);
    E(" xPSR = 0x%08lX", psr);
    E(" CFSR = 0x%08lX", cfsr);
    E(" HFSR = 0x%08lX", hfsr);
    E(" DFSR = 0x%08lX", dfsr);
    E(" AFSR = 0x%08lX", afsr);
    E("MMFAR = 0x%08lX", mmfar);
    E(" BFAR = 0x%08lX", bfar);

    while (1) {
        __asm("BKPT #0");
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static __attribute__((naked)) void HardFault_Handler(void) {
    __asm volatile(
        "TST lr, #4            \n"
        "ITE EQ                \n"
        "MRSEQ r0, MSP         \n"
        "MRSNE r0, PSP         \n"
        "B HardFault_HandlerStackFrame \n");
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void MemManage_Handler(void) {
    E("[MemManage] A memory management fault has occurred!");
    while (1) {
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void BusFault_Handler(void) {
    E("[BusFault] A bus fault has occurred!");
    while (1) {
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void UsageFault_Handler(void) {
    E("[UsageFault] A usage fault has occurred!");
    while (1) {
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static __attribute__((used)) void SVC_HandlerStackFrame(
    uint32_t* stacked_regs) {
    // stacked_regs = R0,R1,R2,R3,R12,LR,PC,xPSR
    uint32_t pc = stacked_regs[6];
    uint16_t* svc_instr = (uint16_t*)(pc - 2);
    uint8_t svc_num = (uint8_t)(*svc_instr & 0xFF);

    uint32_t arg0 = stacked_regs[0];
    uint32_t arg1 = stacked_regs[1];
    void* arg2 = (void*)stacked_regs[2];
    void* arg3 = (void*)stacked_regs[3];
    if (svc_num == SYSCALL_CUSTOM_NUMBER &&
        arg0 < (sizeof(custom_syscall_handlers) /
                sizeof(custom_syscall_handlers[0])) &&
        custom_syscall_handlers[arg0] != NULL) {
        int result = custom_syscall_handlers[arg0](arg0, arg1, arg2, arg3);
        stacked_regs[0] = (uint32_t)result;
    } else {
        stacked_regs[0] = (uint32_t)(-1);
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static __attribute__((naked)) void SVC_Handler(void) {
    __ASM volatile(
        "TST lr, #4            \n"
        "ITE EQ                \n"
        "MRSEQ r0, MSP         \n"
        "MRSNE r0, PSP         \n"
        "B   SVC_HandlerStackFrame     \n");
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void DebugMon_Handler(void) {}
/*--------------------------------------------------------------------------------------------------------------------*/
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
static void SysTick_Handler(void) {
    tick_counter++;
    yield_counter++;
    if (yield_counter >= CONFIG_KERNEL_TICK_TO_YIELD) {
        yield_counter = 0;
        k_task_yield();
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void Irq_Handler(void) {
    uint32_t irq_number = (__get_IPSR() & 0xFF) - 16;
    if ((irq_number < sizeof(irq_handlers) / sizeof(irq_handlers[0])) &&
        irq_handlers[irq_number] != NULL) {
        irq_handlers[irq_number]();
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void k_irq_enable(uint32_t irq_number) {
    NVIC_EnableIRQ((IRQn_Type)((int32_t)(irq_number)-16));
}
/*--------------------------------------------------------------------------------------------------------------------*/
void k_irq_disable(uint32_t irq_number) {
    NVIC_DisableIRQ((IRQn_Type)((int32_t)(irq_number)-16));
}
/*--------------------------------------------------------------------------------------------------------------------*/
void k_irq_set_priority(uint32_t irq_number, uint32_t priority) {
    NVIC_SetPriority((IRQn_Type)((int32_t)(irq_number)-16), priority);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void k_irq_register_handler(uint32_t irq_number, k_irq_handler_t handler) {
    if (irq_number >= NUM_IRQS || handler == NULL) {
        return;
    }
    uint32_t state = k_enter_critical();
    if (!irq_handlers[irq_number]) {
        irq_handlers[irq_number] = handler;
    }
    k_leave_critical(state);
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
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    NVIC_EnableIRQ(PendSV_IRQn);

    if (!scheduler_enabled) {
        scheduler_enabled = true;
    }
    __set_PRIMASK(state);  // leave critical section

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
void k_syscall_register(uint32_t syscall_number, k_syscall_handler_t handler) {
    if (syscall_number >= sizeof(custom_syscall_handlers) /
                              sizeof(custom_syscall_handlers[0]) ||
        handler == NULL) {
        return;
    }
    uint32_t state = k_enter_critical();
    if (!custom_syscall_handlers[syscall_number]) {
        custom_syscall_handlers[syscall_number] = handler;
    }
    k_leave_critical(state);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int k_syscall_invoke(uint32_t arg0, uint32_t arg1, void* arg2, void* arg3) {
    __asm volatile(
        "MOV R0, %0         \n"
        "MOV R1, %1         \n"
        "MOV R2, %2         \n"
        "MOV R3, %3         \n"
        "SVC %4             \n"
        "MOV %0, R0         \n"
        : "+r"(arg0)
        : "r"(arg1), "r"(arg2), "r"(arg3), "I"(SYSCALL_CUSTOM_NUMBER)
        : "r0", "r1", "r2", "r3", "memory");
    return (int)arg0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void k_init(void) {
    I("Kernel init");
    yield_counter = 0;
    SysTick_Config(CONFIG_CPU_CLOCK_HZ / CONFIG_KERNEL_TICK_HZ / 2);
}
/*--------------------------------------------------------------------------------------------------------------------*/
MICROS_REGISTER_INIT_EARLY_FUNCTION(k_init, 0);
/*--------------------------------------------------------------------------------------------------------------------*/
