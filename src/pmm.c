#include "pmm.h"

#define PAGE_SIZE 4096

static uint8_t* bitmap; //map of pages (marked used/unused)
static uint32_t total_pages; //number of pages
static uint32_t mem_size; //size of memory

void pmm_init(uint32_t size)
{
    mem_size = size;	
    total_pages = mem_size / PAGE_SIZE;

    // mark everything as used initially
    for (uint32_t i = 0; i < total_pages / 8; i++)
        bitmap[i] = 0xFF;
}

static void set_bit(uint32_t page)
{
    bitmap[page / 8] |= (1 << (page % 8));
}

static void clear_bit(uint32_t page)
{
    bitmap[page / 8] &= ~(1 << (page % 8));
}

static int test_bit(uint32_t page)
{
    return bitmap[page / 8] & (1 << (page % 8));
}

void* alloc_page()
{
    for (uint32_t i = 0; i < total_pages; i++)
    {
        if (!test_bit(i))
        {
            set_bit(i);
            return (void*)(i * PAGE_SIZE);
        }
    }

    return 0; // no memory
}

void free_page(void* addr)
{
    uint32_t page = (uint32_t)addr / PAGE_SIZE;
    clear_bit(page);
}
