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

struct myfs_inode* myfs_find(const char* name);

int myfs_write(struct myfs_inode* inode, const uint8_t* buffer, uint32_t size);

int myfs_read(struct myfs_inode* inode, uint8_t* buffer, uint32_t size);

struct myfs_inode* myfs_inode(int index);

int myfs_delete(const char* name);
