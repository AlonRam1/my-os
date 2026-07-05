#include <stdint.h>

#define PAGE_SIZE 4096

#define PRESENT 0x1
#define WRITE   0x2

// 4 KB-aligned structures (required by CPU)
static uint32_t page_directory[1024] __attribute__((aligned(4096)));
static uint32_t first_page_table[1024] __attribute__((aligned(4096)));

// Identity map first 4 MB (1024 pages * 4 KB)
static void identity_map_first_4mb(void)
{
    for (int i = 0; i < 1024; i++)
    {
        first_page_table[i] = (i * PAGE_SIZE) | PRESENT | WRITE;
    }

    page_directory[0] = ((uint32_t)first_page_table) | PRESENT | WRITE;
}

// Enable paging (load CR3 + set PG bit in CR0)
static void enable_paging(uint32_t *page_directory_phys)
{
    // Load page directory base register
    asm volatile("mov %0, %%cr3" :: "r"(page_directory_phys));

    // Enable paging (set bit 31 in CR0)
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));

    cr0 |= 0x80000000;

    asm volatile("mov %0, %%cr0" :: "r"(cr0));
}

void paging_init(void)
{
    identity_map_first_4mb();
    enable_paging(page_directory);
}
