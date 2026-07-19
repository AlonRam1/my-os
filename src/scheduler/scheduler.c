#include "scheduler.h"
#include "task/task.h"
#include "timer/timer.h"
#include "tss/tss.h"

#define TIME_QUANTUM 5 //how many ticks before task switch
static uint32_t time_slice = 0; //which tick (modulu TIME_QUANTUM) we are on
volatile int scheduler_lock = 0;

//function that backs up stack state and chooses next task to execute
uint32_t schedule(uint32_t* esp)
{
    wake_tasks();

    time_slice++;

    //if we haven't reached the time quantum to switch tasks, and the current task is still alive, end the function without switching tasks
    if(time_slice < TIME_QUANTUM && tasks[current_task].state != TASK_DEAD)
    {
        return (uint32_t)esp;
    }

    //IF THE ABOVE 'IF' RETURNS FALSE, THAT MEANS WE WANT TO SWITCH TASKS. THE CODE BELOW SWITCHES TASKS

    //if the current task has a stack (aka we are not switching to the first task on the program -> 'current_task' != -1), back up the task stack and switch state to READY
    if (current_task >= 0)
    {
        tasks[current_task].esp = (uint32_t)esp;

        if(tasks[current_task].state == TASK_RUNNING)
            tasks[current_task].state = TASK_READY;
    }

    int found = 0; //on task switch, check if found READY tasks to switch to

    //search for READY tasks
    for(int i = 0; i < task_count; i++)
    {
        current_task++;

        if(current_task >= task_count)
            current_task = 0;

        if(tasks[current_task].state == TASK_READY)
        {
            found = 1;
            break;
        }
    }
    
    //if we didn't find a READY task, switch to idle task
    if(!found)
    {
        for(int i = 0; i < task_count; i++)
        {
            if(tasks[i].state == TASK_IDLE)
            {
                current_task = i;
                break;
            }
        }
    }

    time_slice = 0;

    //switch new task state to running, and return new task's stack pointer to the asm handler to do the actual CPU task switch
    if(tasks[current_task].state != TASK_IDLE)
        tasks[current_task].state = TASK_RUNNING;

    tss_set_kernel_stack(tasks[current_task].kernel_esp0);
    return tasks[current_task].esp;
}

