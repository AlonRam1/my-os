#include <user/commands.h>
#include <kernel/terminal.h>
#include <vga/vga.h>
#include <kernel/string/string.h>
#include <kernel/fs/myfs.h>
#include <kernel/fs/block.h>
#include <stdint.h>

static void echo_handler(const char* args);
static void ls_handler(const char* args);
static void cat_handler(const char* args);
static void clear_handler(const char* args);
static void touch_handler(const char* args);
static void rm_handler(const char* args);
static void mkdir_handler(const char* args);
static void cd_handler(const char* args);


const struct command commands[] =
{
    { "echo", echo_handler },
    { "ls", ls_handler },
    { "clear", clear_handler },
    { "touch", touch_handler },
    { "rm", rm_handler },
    { "mkdir", mkdir_handler },
    { "cd", cd_handler },
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

static void ls_handler(const char* args)
{
    if(args)
    {
        terminal_write("ls: too many arguments\n");
        return;
    }

    struct myfs_inode* dir = myfs_inode(current_directory);

    if(!dir)
    {
        terminal_write("ls: invalid directory\n");
        return;
    }

    uint8_t block[MYFS_BLOCK_SIZE];

    block_read(dir->block, block);

    struct myfs_dir_entry* entries = (struct myfs_dir_entry*)block;

    for(int i = 0; i < MYFS_BLOCK_SIZE / sizeof(struct myfs_dir_entry); i++)
    {
        if(entries[i].inode != 0)
        {
            terminal_write(entries[i].name);
            terminal_write("\n");
        }
    }
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

static void touch_handler(const char* args)
{
	if(args_empty(args))
	{
		terminal_write("touch: argument missing\n");
		return;
	}
	myfs_create_file(args, current_directory);
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

	//delete component from filesystem
	myfs_delete(args);
	
}

static void mkdir_handler(const char* args)
{
	if(args_empty(args))
	{
		terminal_write("mkdir: argument missing\n");
		return;
	}
	myfs_create_directory(args, current_directory);

}

static void cd_handler(const char* args)
{
    if(args_empty(args))
    {
        terminal_write("cd: argument missing\n");
        return;
    }

    if(streq(args, ".."))
    {
        if(current_directory != 0)
        {
            struct myfs_inode* current = myfs_inode(current_directory);

            current_directory = current->parent;
        }

        return;
    }

    struct myfs_inode* dir = myfs_find_in_directory(current_directory, args);

    if(!dir)
    {
        terminal_write("cd: directory not found\n");
        return;
    }

    if(dir->type != MYFS_DIRECTORY)
    {
        terminal_write("cd: not a directory\n");
        return;
    }

    current_directory = dir - myfs_inode(0);
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
