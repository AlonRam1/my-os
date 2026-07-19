#include "syscall.h"
#include "vga/vga.h"

void sys_write(char ebx)
{
	putchar(ebx);
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
            puts("SYS_EXIT\n");
            while (1);
            break;

        default:
            puts("UNKNOWN SYSCALL\n");
            break;
    }
}


