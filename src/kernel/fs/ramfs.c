#include <kernel/fs/ramfs.h>
#include <kernel/string/string.h>

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

//write data into a file
int ramfs_write(struct ramfs_file* file, uint32_t offset, const char* buffer, uint32_t size)
{
    if(!file)
        return -1;

    //can't write past end of file buffer
    if(offset >= RAMFS_FILE_SIZE)
        return 0;

    //truncate write if it would exceed file size
    if(offset + size > RAMFS_FILE_SIZE)
        size = RAMFS_FILE_SIZE - offset;

    for(uint32_t i = 0; i < size; i++)
    {
        file->data[offset + i] = buffer[i];
    }

    //extend file size if necessary
    if(offset + size > file->size)
    {
        file->size = offset + size;
    }

    return size;
}

//read data from a file
int ramfs_read(struct ramfs_file* file, uint32_t offset, char* buffer, uint32_t size)
{
    if(!file)
        return -1;

    //can't read past end of file
    if(offset >= file->size)
        return 0;

    //truncate read if it would exceed file size
    if(offset + size > file->size)
        size = file->size - offset;

    for(uint32_t i = 0; i < size; i++)
    {
        buffer[i] = file->data[offset + i];
    }

    return size;
}
