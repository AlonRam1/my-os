#pragma once
#include <stdint.h>


void pmm_init(uint32_t size);
void* alloc_page(void);
void free_page(void* addr);

