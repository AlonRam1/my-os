#include "task.h"
#include "vga/vga.h"
#include "timer/timer.h"
#include <stdint.h>

static uint8_t stacks[MAX_TASKS][STACK_SIZE]; //we back up our task stacks here (each row is a stack)
int current_task = -1;
int task_count = 0;
struct task tasks[MAX_TASKS];


//this task runs by default when all other tasks are disabled
static void task_idle()
{
    while(1)
    {
        asm volatile("hlt");
    }
}

//enable tasks
void task_init()
{
    for (int i = 0; i < MAX_TASKS; i++)
    {
        tasks[i].state = TASK_DEAD;
        tasks[i].id = i;
        tasks[i].esp = 0;
        tasks[i].wake_tick = 0;
    }

    //create fallback idle task for when there are no tasks running
    int idle = task_create(task_idle);
    tasks[idle].state = TASK_IDLE;
}

//create kernel task
int task_create(void (*entry)())
{
    if (task_count >= MAX_TASKS)
        return -1;

    int id = task_count;
    uint32_t* stack = (uint32_t*)&stacks[id][STACK_SIZE]; //stack variable starts at the top of the task's stack

    //set up task stack
    *(--stack) = 0x202; //EFLAGS (interrupt enabled)
    *(--stack) = 0x08; //kernel code segment (CS)
    *(--stack) = (uint32_t)entry; //EIP
    *(--stack) = 0; //eax
    *(--stack) = 0; //ecx
    *(--stack) = 0; //edx
    *(--stack) = 0; //ebx
    *(--stack) = 0; //esp
    *(--stack) = 0; //ebp
    *(--stack) = 0; //esi
    *(--stack) = 0; //edi

    //initialize task fields
    tasks[id].esp = (uint32_t)stack;
    tasks[id].id = id;
    tasks[id].state = TASK_READY;
    tasks[id].wake_tick = 0;

    task_count++;

    return id;
}

//create user task
int task_create_user(void (*entry)())
{
    if (task_count >= MAX_TASKS)
        return -1;

    int id = task_count;
    uint32_t* stack = (uint32_t*)&stacks[id][STACK_SIZE]; //stack variable starts at the top of the task's stack

    //set up task stack (+push user esp and ss to switch rings)
    *(--stack) = 0x23; //user SS
    *(--stack) = 0x90000; //user ESP
    *(--stack) = 0x202; //EFLAGS (interrupt enabled)
    *(--stack) = 0x1B; //user code segment (CS)
    *(--stack) = (uint32_t)entry; //eip to task (given as argument)
    *(--stack) = 0; //eax
    *(--stack) = 0; //ecx
    *(--stack) = 0; //edx
    *(--stack) = 0; //ebx
    *(--stack) = 0; //esp
    *(--stack) = 0; //ebp
    *(--stack) = 0; //esi
    *(--stack) = 0; //edi
	
    //initialize task fields
    tasks[id].esp = (uint32_t)stack;
    tasks[id].id = id;
    tasks[id].state = TASK_READY;
    tasks[id].wake_tick = 0;

    task_count++;

    return id;
}

//task sleep
void task_sleep(uint32_t duration)
{
    tasks[current_task].wake_tick = ticks + duration; //set wake tick
    tasks[current_task].state = TASK_BLOCKED; //set task to block
    task_yield(); //immediately switch tasks
}

//wake all tasks that reached their wake tick
void wake_tasks()
{
    for(int i = 0; i < task_count; i++)
    {
        if(tasks[i].state == TASK_BLOCKED && ticks >= tasks[i].wake_tick)
        {
            tasks[i].state = TASK_READY;
        }
    }
}

//switch tasks
void task_yield()
{
    asm volatile("int $32");
}

//kill task
void task_exit()
{
    tasks[current_task].state = TASK_DEAD;
    task_yield(); 
}
