#include <stdint.h>
#include "user.h"

void syscall_write(char c)
{
    uint32_t value = (uint32_t)c;
    //call int128 with the relevant arguments for the write syscall
    asm volatile("mov $1, %%eax; mov %0, %%ebx; int $128" :: "r"(value) : "eax", "ebx");
}

void user_test()
{
    syscall_write('X');

    while(1)
    {
        asm volatile("nop");
    }
}
