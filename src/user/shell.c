#include <user/shell.h>
#include <kernel/terminal.h>
#include <kernel/syscall.h>
#include <stdint.h>


void shell_task()
{
    char buffer[64]; //buffer to write into from terminal buffer 

    terminal_write("MYOS shell started\n> "); //test

    while(1)
    {
        int count = terminal_readline(buffer, 64); //read from terminal into our buffer
        if(count > 0)
        {
            terminal_write("\n");
            terminal_write(buffer);
            terminal_write("\n> ");
        }

        asm volatile("int $0x80" : : "a"(SYS_YIELD)); //call syscall "yield" 
    }
}
