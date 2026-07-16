#include "scheduler.h"
#include "task/task.h"
#include "timer/timer.h"

#define TIME_QUANTUM 5 //how many ticks before task switch
static uint32_t time_slice = 0; //which tick (modulu TIME_QUANTUM) we are on

//function that backs up stack state and chooses next task to execute
uint32_t schedule(uint32_t* esp)
{
    wake_tasks();

    time_slice++;

    if(time_slice < TIME_QUANTUM && tasks[current_task].state != TASK_DEAD)
    {
        return (uint32_t)esp;
    }

    if (current_task >= 0)
    {
        tasks[current_task].esp = (uint32_t)esp;

        if(tasks[current_task].state == TASK_RUNNING)
            tasks[current_task].state = TASK_READY;
    }

    int found = 0; //on task switch, check if found READY tasks to switch to

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

    tasks[current_task].state = TASK_RUNNING;
    return tasks[current_task].esp;
}

