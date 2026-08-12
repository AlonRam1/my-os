#include <stdint.h>
#include <user/user.h>

void syscall_write(char c)
{
    uint32_t value = (uint32_t)c;
    asm volatile("mov $1, %%eax; mov %0, %%ebx; int $128" :: "r"(value) : "eax", "ebx");
}

void syscall_write_string(const char* string)
{
    asm volatile("mov $5, %%eax; mov %0, %%ebx; int $128" :: "r"(string) : "eax", "ebx", "memory");
}

void syscall_exit()
{
    asm volatile("mov $0, %%eax; int $128" ::: "eax");
}

void syscall_yield()
{
    asm volatile("mov $2, %%eax; int $128" ::: "eax");
}

void syscall_sleep(uint32_t ticks)
{
    asm volatile("mov $3, %%eax; mov %0, %%ebx; int $128" :: "r"(ticks) : "eax", "ebx");
}

int syscall_create_file(const char* name)
{
    int result;
    asm volatile("mov $6, %%eax; mov %1, %%ebx; int $128; mov %%eax, %0" : "=r"(result) : "r"(name) : "eax", "ebx", "memory");
    return result;
}

int syscall_delete_file(const char* name)
{
    int result;
    asm volatile("mov $7, %%eax; mov %1, %%ebx; int $128; mov %%eax, %0" : "=r"(result) : "r"(name) : "eax", "ebx", "memory");
    return result;
}

int syscall_create_directory(const char* name)
{
    int result;
    asm volatile("mov $8, %%eax; mov %1, %%ebx; int $128; mov %%eax, %0" : "=r"(result) : "r"(name) : "eax", "ebx", "memory");
    return result;
}

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
