#include <user/commands.h>
#include <kernel/terminal.h>
#include <vga/vga.h>
#include <kernel/string/string.h>
#include <kernel/fs/myfs.h>
#include <stdint.h>

static void echo_handler(const char* args);
static void ls_handler(const char* args);
static void cat_handler(const char* args);
static void clear_handler(const char* args);
static void touch_handler(const char* args);
static void rm_handler(const char* args);

const struct command commands[] =
{
    { "echo", echo_handler },
    { "ls", ls_handler },
    { "cat", cat_handler },
    { "clear", clear_handler },
    { "touch", touch_handler },
    { "rm", rm_handler },
};

const uint32_t command_count = sizeof(commands) / sizeof(commands[0]);

//detect empty args
int args_empty(char* arg)
{
    if(arg == 0)
        return 1;

    while(*arg == ' ')
    {
        arg++;
    }

    if(*arg == 0)
    {
        return 1;
    }
    return 0;
}

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
	
	for(int i = 0; i < MYFS_MAX_FILES; i++)
        {
            struct myfs_inode* file = myfs_inode(i);

            if(file->used)
            {
                terminal_write(file->name);
                terminal_write("\n");
            }
        }
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

static void touch_handler(const char* args)
{
	if(args_empty(args))
	{
		terminal_write("touch: argument missing\n");
		return;
	}
	myfs_create(args);
}

static void rm_handler(const char* args)
{

	if(args_empty(args))
	{
		terminal_write("touch: argument missing\n");
		return;
	}

	if(!myfs_find(args))
	{
		terminal_write("rm: file does not exist\n");
		return;
	}
	myfs_delete(args);
}

