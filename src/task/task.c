#include "task.h"
#include "vga/vga.h"
#include <stdint.h>

static struct task tasks[MAX_TASKS]; //array of tasks
static uint8_t stacks[MAX_TASKS][STACK_SIZE]; //we back up our task stacks here (each row is a stack)

static int current_task = -1;
static int task_count = 0;

void task_exit(void)
{
    tasks[current_task].state = TASK_DEAD;

    while (1)
        asm volatile("hlt");
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

    uint32_t* stack = (uint32_t*)&stacks[id][STACK_SIZE]; //stack variable starts at the top of the task's stack

    //set up task stack
    *(--stack) = 0x202; //EFLAGS (interrupt enabled)
    *(--stack) = 0x10; //kernel code segment
    *(--stack) = (uint32_t)entry; //EIP
    *(--stack) = 0; //eax
    *(--stack) = 0; //ecx
    *(--stack) = 0; //edx
    *(--stack) = 0; //ebx
    *(--stack) = 0; //esp
    *(--stack) = 0; //ebp
    *(--stack) = 0; //esi
    *(--stack) = 0; //edi

    tasks[id].esp = (uint32_t)stack;
    tasks[id].id = id;
    tasks[id].state = TASK_READY;

    task_count++;

    return id;
}

void task_yield()
{
    schedule();
}

void schedule()
{
    int previous = current_task;

    current_task++;

    if (current_task >= task_count)
	current_task = 0;

    tasks[current_task].state = TASK_RUNNING;

    if (previous == -1)
    {
	switch_interrupt_context(tasks[current_task].esp);
    }
    else
    {
	tasks[previous].state = TASK_READY;    
        switch_task(&tasks[previous].esp,tasks[current_task].esp);
    }
}

//function that backs up stack state and chooses next task to execute
uint32_t schedule_interrupt(uint32_t* esp)
{
    //backup current task's stack	
    if (current_task >= 0)
        tasks[current_task].esp = (uint32_t)esp;

    current_task++;

    if (current_task >= task_count)
        current_task = 0;

    tasks[current_task].state = TASK_RUNNING;

    return tasks[current_task].esp;
}


