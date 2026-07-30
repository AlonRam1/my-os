#pragma once

#include <stdint.h>
#include <kernel/fs/myfs.h>

#define VFS_MAX_FILES 32

struct file_descriptor
{
    struct myfs_inode* file;
    uint32_t position;
    int used;
};

void vfs_init();

int vfs_create(const char* name);

int vfs_open(const char* name);

int vfs_read(int fd, char* buffer, uint32_t size);

int vfs_write(int fd, const char* buffer, uint32_t size);

void vfs_close(int fd);
