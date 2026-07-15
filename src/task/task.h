#pragma once

#include <stdint.h>

#define MAX_TASKS 8
#define STACK_SIZE 4096

#define TASK_READY 0
#define TASK_RUNNING 1
#define TASK_DEAD 2

struct task
{
    uint32_t esp; //task's stack pointer (for when we want to context switch)
    uint32_t id;
    int state; 
};

void task_init();
int task_create(void (*entry)());
void task_exit();
void task_yield();
void schedule();
uint32_t schedule_interrupt(uint32_t* esp);
void switch_task(uint32_t* old_esp, uint32_t new_esp);
void switch_interrupt_context(uint32_t new_esp);

