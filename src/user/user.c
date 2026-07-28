#include <stdint.h>
#include <user/user.h>

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

void syscall_yield()
{
    asm volatile("mov $2, %%eax; int $128" ::: "eax");
}

void syscall_sleep(uint32_t ticks)
{
    asm volatile("mov $3, %%eax;" "mov %0, %%ebx;" "int $128" : : "r"(ticks) : "eax", "ebx");}

void user_test2()
{
    syscall_write('X');   
}   

void user_test1()
{
    syscall_write('A');

    syscall_yield();

    syscall_write('B');

    syscall_sleep(10);

    syscall_write('C');

    syscall_exit();   
}

void user_exit_stub()
{
    syscall_exit();
    while(1)
    {
        asm volatile("nop");
    }
}
