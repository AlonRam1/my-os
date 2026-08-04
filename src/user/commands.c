#include <user/commands.h>
#include <kernel/terminal.h>
#include <vga/vga.h>
#include <kernel/string/string.h>
#include <stdint.h>

static void echo_handler(const char* args);
static void ls_handler(const char* args);
static void cat_handler(const char* args);
static void clear_handler(const char* args);

const struct command commands[] =
{
    { "echo", echo_handler },
    { "ls", ls_handler },
    { "cat", cat_handler },
    { "clear", clear_handler },
};

const uint32_t command_count = sizeof(commands) / sizeof(commands[0]);

void echo_handler(const char* args)
{
	if(args)
            terminal_write(args);
        terminal_write("\n");
}

void ls_handler(const char* args)
{
	if(args)
    	{
        	terminal_write("ls: too many arguments\n");
        	return;
    	}
	terminal_write("ls command\n");
}

static void cat_handler(const char* args)
{
	terminal_write("cat command\n");
}

static void clear_handler(const char* args)
{
	if(args)
    	{
        	terminal_write("clear: too many arguments\n");
        	return;
    	}
	clearscreen();
}

void execute(char* name, char* args)
{
	for(int i = 0; i < command_count ; i++)
	{
		if(streq(commands[i].name, name))
		{
			commands[i].execute(args);
			return;
		}
	}
	terminal_write("command not found\n");
}

