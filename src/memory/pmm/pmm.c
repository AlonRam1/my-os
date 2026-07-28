#include <memory/pmm.h>
#include <stdint.h>

#define PAGE_SIZE 4096
#define MAX_MEMORY (32 * 1024 * 1024)

static uint8_t bitmap[MAX_MEMORY / PAGE_SIZE / 8];

static uint32_t total_pages;
static uint32_t mem_size;

static void set_bit(uint32_t page);
static void clear_bit(uint32_t page);
static int test_bit(uint32_t page);

void pmm_init(uint32_t size)
{
    mem_size = size;
    total_pages = mem_size / PAGE_SIZE;

    // IMPORTANT: initialize bitmap to 0 (all free)
    for (uint32_t i = 0; i < (MAX_MEMORY / PAGE_SIZE / 8); i++)
        bitmap[i] = 0x00;

    // reserve first 4MB (identity mapped + kernel safety)
    for (uint32_t i = 0; i < 1024; i++)
        set_bit(i);
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

    return 0;
}

void free_page(void* addr)
{
    uint32_t page = (uint32_t)addr / PAGE_SIZE;
    clear_bit(page);
}
