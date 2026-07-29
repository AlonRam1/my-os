#pragma once

#include <stdint.h>

#define BLOCK_SIZE 512

int block_read(uint32_t block, uint8_t* buffer);
int block_write(uint32_t block, uint8_t* buffer);
