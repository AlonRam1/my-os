#include <user/shell.h>
#include <kernel/terminal.h>
#include <kernel/syscall.h>
#include <kernel/string/string.h>
#include <stdint.h>

//parse given string into command and parameters
static void shell_parse(char* line, char** command, char** argument)
{
    *command = line;
    *argument = 0;

    for(int i = 0; line[i]; i++)
    {
        if(line[i] == ' ')
        {
            line[i] = 0;
            *argument = &line[i + 1];
            return;
        }
    }
}

static void shell_execute(const char* command, const char* argument)
{
    if(streq(command, "echo"))
    {
        if(argument)
            terminal_write(argument);

        terminal_write("\n");
    }
    else if(streq(command, "ls"))
    {
        terminal_write("ls command\n");
    }
    else if(streq(command, "cat"))
    {
        terminal_write("cat command\n");
    }
    else
    {
        terminal_write("Unknown command\n");
    }
}

void shell_task()
{
    char buffer[64];

    while(1)
    {
        int count = terminal_readline(buffer, 64); //read command from user

        terminal_write("\n");

        if(count > 0)
        {
            char* command;
            char* arguments;

            shell_parse(buffer, &command, &arguments); //parse into command + parameters
            shell_execute(command, arguments); //execute the command
        }

        terminal_write("> ");

        asm volatile("int $0x80" : : "a"(SYS_YIELD)); //call sys_yield
    }
}
