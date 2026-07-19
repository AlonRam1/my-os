#pragma once

#include <stdint.h>

#define MAX_TASKS 8
#define STACK_SIZE 4096

#define TASK_READY 0
#define TASK_RUNNING 1
#define TASK_BLOCKED 2
#define TASK_DEAD 3
#define TASK_IDLE 4

extern int current_task;
extern int task_count;

struct task
{
    uint32_t esp; //task's stack pointer (for when we want to context switch)
    uint32_t id;
    int state;
    uint32_t wake_tick;
};

extern struct task tasks[]; //array of tasks

void task_init();
int task_create(void (*entry)());
void task_exit();
void task_sleep(uint32_t duration);
void wake_tasks();
void task_yield();
int task_create_user(void (*entry)());
