/* SPDX-License-Identifier: MIT */
/*--------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "micros/kernel.h"
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
        printf("%s cnt=%d\n", info->name, cnt++);
        k_delay_ms(info->delay_ms);
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    printf("Hello, World!\n");
    printf("This is MicrOS running on SOME hardware platform.\n");

    k_task_create(task_function, &(task_info_t){"Task 1 -->", 333}, stack1,
                  sizeof(stack1));
    k_task_create(task_function, &(task_info_t){"Task 2 <--", 500}, stack2,
                  sizeof(stack2));

    k_scheduler_start();
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
