#include <kernel/syscall.h>
#include <kernel/task.h>
#include <kernel/terminal.h>
#include <kernel/fs/myfs.h>
#include <drivers/keyboard/keyboard.h>
#include <vga/vga.h>
#include <stdint.h>

static void sys_write(char c)
{
    putchar(c);
}

static void sys_write_string(const char* string)
{
    if(!string)
        return;

    terminal_write(string);
}

static void sys_exit()
{
    task_exit();
}

static void sys_yield()
{
    task_yield();
}

static void sys_sleep(uint32_t ticks)
{
    task_sleep(ticks);
}

static void sys_read(char* buffer)
{
    if(!buffer)
        return;

    keyboard_read(buffer, 128);
}

static int sys_create_file(const char* name)
{
    if(!name)
        return -1;

    return myfs_create_file(name, current_directory);
}

static int sys_delete_file(const char* name)
{
    if(!name)
        return -1;

    return myfs_delete(name);
}

static int sys_create_directory(const char* name)
{
    if(!name)
        return -1;

    return myfs_create_directory(name, current_directory);
}

void syscall_handler(uint32_t* regs)
{
    uint32_t eax = regs[7];
    uint32_t ebx = regs[4];

    switch(eax)
    {
        case SYS_EXIT:
            sys_exit();
            break;

        case SYS_WRITE:
            sys_write((char)ebx);
            break;

        case SYS_YIELD:
            sys_yield();
            break;

        case SYS_SLEEP:
            sys_sleep(ebx);
            break;

        case SYS_READ:
            sys_read((char*)ebx);
            break;

        case SYS_WRITE_STRING:
            sys_write_string((char*)ebx);
            break;

        case SYS_CREATE_FILE:
            sys_create_file((char*)ebx);
            break;

        case SYS_DELETE_FILE:
            sys_delete_file((char*)ebx);
            break;

        case SYS_CREATE_DIRECTORY:
            sys_create_directory((char*)ebx);
            break;

        default:
            terminal_write("UNKNOWN SYSCALL\n");
            break;
    }
}
