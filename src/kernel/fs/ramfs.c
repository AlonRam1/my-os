#include <kernel/fs/ramfs.h>

static struct ramfs_file files[RAMFS_MAX_FILES]; //table of all files

//initialize files array (zero out)
void ramfs_init()
{
    for(int i = 0; i < RAMFS_MAX_FILES; i++)
    {
        files[i].used = 0;
        files[i].size = 0;
    }
}

//our own string-equal (reminder: we do not have libc available in a kernel project)
static int streq(const char* a, const char* b)
{
    while(*a || *b)
    {
        if(*a != *b)
            return 0;

        a++;
        b++;
    }

    return 1;
}

//our own string-copy
static void strcopy(char* dst, const char* src)
{
    while(*src)
    {
        *dst++ = *src++;
    }

    *dst = 0;
}

//find a file in the files array by name
struct ramfs_file* ramfs_find(const char* name)
{
    for(int i = 0; i < RAMFS_MAX_FILES; i++)
    {
        if(files[i].used && streq(files[i].name, name))
            return &files[i];
    }

    return 0;
}

//create a new file
int ramfs_create(const char* name)
{
    if(ramfs_find(name))
        return -1;

    for(int i = 0; i < RAMFS_MAX_FILES; i++)
    {
        if(!files[i].used)
        {
            files[i].used = 1;
            files[i].size = 0;

            strcopy(files[i].name, name);

            return 0;
        }
    }

    return -1;
}
