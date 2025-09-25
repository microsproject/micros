/*--------------------------------------------------------------------------------------------------------------------*/
/* SPDX-License-Identifier: MIT */
/*--------------------------------------------------------------------------------------------------------------------*/
#define CONFIG_MICROS_LOG_ENABLE
#define CONFIG_MICROS_LOG_LEVEL MICROS_LOG_LEVEL_DEBUG
/*--------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "micros/kernel.h"
#include "micros/log.h"
/*--------------------------------------------------------------------------------------------------------------------*/
MICROS_LOG_REGISTER(main, MICROS_LOG_LEVEL_INFO);
/*--------------------------------------------------------------------------------------------------------------------*/
static uint32_t stack1[128];
static uint32_t stack2[128];
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct {
    const char* name;
    int delay_ms;
} task_info_t;
/*--------------------------------------------------------------------------------------------------------------------*/
static void task_function(void* arg) {
    task_info_t* info = (task_info_t*)arg;
    int cnt = 0;
    while (1) {
        I("%s cnt=%d", info->name, cnt++);
        k_delay_ms(info->delay_ms);
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    I("Hello, World!");
    I("This is MicrOS running on SOME hardware platform.");

    k_task_create(task_function, &(task_info_t){"Task 1 -->", 333}, stack1,
                  sizeof(stack1));
    k_task_create(task_function, &(task_info_t){"Task 2 <--", 500}, stack2,
                  sizeof(stack2));

    k_scheduler_start();
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
