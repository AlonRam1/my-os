#pragma once
#include <stdint.h>

void pmm_init(uint32_t mem_size);

void* alloc_page();
void free_page(void* addr);
