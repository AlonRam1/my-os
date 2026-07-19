#include "syscall.h"
#include "task/task.h"
#include "vga/vga.h"

void sys_write(char ebx)
{
	putchar(ebx);
}

void sys_exit()
{
	putchar('U');
	task_exit();
}

void syscall_handler(uint32_t* regs)
{
    uint32_t eax = regs[7];
    uint32_t ebx = regs[4];

    switch (eax)
    {
        case 1:
            sys_write((char)ebx);				
            break;

        case 0:
            sys_exit();
            break;

        default:
            puts("UNKNOWN SYSCALL\n");
            break;
    }
}


