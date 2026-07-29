#pragma once

#include <stdint.h>

#define MYFS_MAGIC 0x4D594653

#define MYFS_MAX_FILES 32
#define MYFS_NAME_LEN 32

#define MYFS_BLOCK_SIZE 512

#define MYFS_INODE_START 1
#define MYFS_DATA_START 9


struct myfs_superblock
{
    uint32_t magic;
    uint32_t total_blocks;
    uint32_t free_blocks;
};


struct myfs_inode
{
    char name[MYFS_NAME_LEN];

    uint32_t size;

    uint32_t block;

    uint32_t used;
};


void myfs_format(uint32_t total_blocks);

int myfs_mount();

int myfs_create(const char* name);
