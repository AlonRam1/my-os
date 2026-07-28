#include <kernel/syscall.h>
#include <kernel/task.h>
#include <vga/vga.h>

void sys_write(char ebx)
{
	putchar(ebx);
}

void sys_exit()
{
	task_exit();
}

void sys_yield()
{
	task_yield();
}

void sys_sleep(uint32_t ebx)
{
	task_sleep(ebx);
}

void syscall_handler(uint32_t* regs)
{
    uint32_t eax = regs[7];
    uint32_t ebx = regs[4];

    switch (eax)
    {
        case SYS_WRITE:
            sys_write((char)ebx);				
            break;

        case SYS_EXIT:
            sys_exit();
            break;

	case SYS_YIELD:
            sys_yield();
            break;
	 
	case SYS_SLEEP:
            sys_sleep(ebx);
            break;    

        default:
            puts("UNKNOWN SYSCALL\n");
            break;
    }
}


