#include <stdint.h>
#include "user.h"

void syscall_write(char c)
{
    uint32_t value = (uint32_t)c;
    //call int128 with the relevant arguments for the write syscall
    asm volatile("mov $1, %%eax; mov %0, %%ebx; int $128" :: "r"(value) : "eax", "ebx");
}

void syscall_exit()
{
    //call int128 with the relevant arguments for the exit syscall	
    asm volatile("mov $0, %%eax; int $128" ::: "eax");
}

void user_test1()
{
    while(1)
    {
    	syscall_write('X');
    }
}   

void user_test2()
{
    while(1)
    {
    	syscall_write('Y');
    }
}

void user_exit_stub()
{
    syscall_exit();
    while(1)
    {
        asm volatile("nop");
    }
}
