#pragma once

#include <stdint.h>

#define MYFS_MAGIC 0x4D594653

#define MYFS_FILE 1
#define MYFS_DIRECTORY 2

#define MYFS_MAX_FILES 32
#define MYFS_NAME_LEN 32

#define MYFS_BLOCK_SIZE 512

#define MYFS_INODE_START 1
#define MYFS_DATA_START 9

extern uint32_t current_directory;

struct myfs_superblock
{
    uint32_t magic;
    uint32_t total_blocks;
    uint32_t free_blocks;
};


struct myfs_inode
{
    int used;

    char name[MYFS_NAME_LEN];

    uint32_t size;
    uint32_t block;

    uint32_t parent;
    uint8_t type;
};

struct myfs_dir_entry
{
    uint32_t inode;
    uint8_t used;
    char name[MYFS_NAME_LEN];
};

void myfs_format(uint32_t total_blocks);

int myfs_mount();

int myfs_create_file(const char* name, uint32_t);

struct myfs_inode* myfs_find(const char* name);

int myfs_write(struct myfs_inode* inode, const uint8_t* buffer, uint32_t size);

int myfs_read(struct myfs_inode* inode, uint8_t* buffer, uint32_t size);

struct myfs_inode* myfs_inode(int index);

int myfs_delete(const char* name);

int myfs_create_directory(const char* name, uint32_t parent);

int myfs_add_directory_entry(uint32_t directory, uint32_t inode, const char* name);

struct myfs_inode* myfs_find_in_directory(uint32_t parent, const char* name);
