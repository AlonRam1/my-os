#pragma once

#include <stdint.h>

#define RAMFS_MAX_FILES 32
#define RAMFS_NAME_LEN 32
#define RAMFS_FILE_SIZE 4096

struct ramfs_file
{
    char name[RAMFS_NAME_LEN];
    uint8_t data[RAMFS_FILE_SIZE];
    uint32_t size;
    int used;
};

void ramfs_init();

int ramfs_create(const char* name);

struct ramfs_file* ramfs_find(const char* name);
