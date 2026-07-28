#pragma once
#include <stdint.h>

void paging_init(void);

void map_page(uint32_t virt, uint32_t phys, uint32_t flags);

void unmap_page(uint32_t virt);
