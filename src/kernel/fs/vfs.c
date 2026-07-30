#include <kernel/fs/vfs.h>

static struct file_descriptor fd_table[VFS_MAX_FILES];

void vfs_init()
{
    for(int i = 0; i < VFS_MAX_FILES; i++)
    {
        fd_table[i].used = 0;
        fd_table[i].file = 0;
        fd_table[i].position = 0;
    }
}

int vfs_create(const char* name)
{
    return myfs_create(name);
}

int vfs_open(const char* name)
{
    struct myfs_inode* file = myfs_find(name);

    if(!file)
        return -1;

    for(int i = 0; i < VFS_MAX_FILES; i++)
    {
        if(!fd_table[i].used)
        {
            fd_table[i].used = 1;
            fd_table[i].file = file;
            fd_table[i].position = 0;

            return i;
        }
    }

    return -1;
}

int vfs_read(int fd, char* buffer, uint32_t size)
{
    if(fd < 0 || fd >= VFS_MAX_FILES)
        return -1;

    if(!fd_table[fd].used)
        return -1;

    int bytes = myfs_read(
        fd_table[fd].file,
        (uint8_t*)buffer,
        size
    );

    if(bytes > 0)
    {
        fd_table[fd].position += bytes;
    }

    return bytes;
}

int vfs_write(int fd, const char* buffer, uint32_t size)
{
    if(fd < 0 || fd >= VFS_MAX_FILES)
        return -1;

    if(!fd_table[fd].used)
        return -1;

    int bytes = myfs_write(
        fd_table[fd].file,
        (const uint8_t*)buffer,
        size
    );

    if(bytes > 0)
    {
        fd_table[fd].position += bytes;
    }

    return bytes;
}

void vfs_close(int fd)
{
    if(fd < 0 || fd >= VFS_MAX_FILES)
        return;

    fd_table[fd].used = 0;
    fd_table[fd].file = 0;
    fd_table[fd].position = 0;
}
