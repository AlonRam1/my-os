#include <user/shell.h>
#include <kernel/terminal.h>
#include <kernel/syscall.h>
#include <kernel/string/string.h>
#include <stdint.h>


void shell_task()
{
    char buffer[64];

    terminal_write("MYOS shell started\n> ");

    while(1)
    {
        int count = terminal_readline(buffer, 64);

        terminal_write("\n");

        if(count > 0)
        {
            if(streq(buffer, "echo"))
            {
                terminal_write("echo command\n");
            }
            else if(streq(buffer, "ls"))
            {
                terminal_write("ls command\n");
            }
            else if(streq(buffer, "cat"))
            {
                terminal_write("cat command\n");
            }
            else
            {
                terminal_write("Unknown command\n");
            }
        }

        terminal_write("> ");

        asm volatile("int $0x80" : : "a"(SYS_YIELD));
    }
}
