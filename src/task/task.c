#include "task.h"
#include <stdint.h>

extern void switch_task(uint32_t* old_esp, uint32_t new_esp);


static struct task tasks[MAX_TASKS]; //array of tasks
static uint8_t stacks[MAX_TASKS][STACK_SIZE]; //we back up our task stacks here (each row is a stack)

static int current_task = -1;
static int task_count = 0;

void task_exit()
{
    tasks[current_task].state = TASK_READY;

    task_yield();

    while(1);
}


void task_init()
{
    for (int i = 0; i < MAX_TASKS; i++)
    {
        tasks[i].state = TASK_READY;
    }
}

int task_create(void (*entry)())
{
    if (task_count >= MAX_TASKS)
        return -1;

    int id = task_count;

    uint32_t* stack = (uint32_t*)&stacks[id][STACK_SIZE]; //initialize "stack" variable to bottom of this task's stack (remember little endian)

    //initialize stack for task
    *(--stack) = (uint32_t)entry; // return address for ret
    *(--stack) = 0; // eax
    *(--stack) = 0; // ecx
    *(--stack) = 0; // edx
    *(--stack) = 0; // ebx
    *(--stack) = 0; // esp 
    *(--stack) = 0; // ebp
    *(--stack) = 0; // esi
    *(--stack) = 0; // edi

    tasks[id].esp = (uint32_t)stack;
    tasks[id].id = id;
    tasks[id].state = TASK_READY;

    task_count++;

    return id;
}

void task_yield()
{
    int previous = current_task;

    current_task++;

    if (current_task >= task_count)
        current_task = 0;

    if (previous == -1) //if this is the first task to be yielded (since current_task is initialized as -1)

    {
        switch_task(0, tasks[current_task].esp); //NOTE: you have to create atleast one task before running this, or else the stack will be filled with garbage
    }
    else
    {
        switch_task(&tasks[previous].esp, tasks[current_task].esp);
    }
}
