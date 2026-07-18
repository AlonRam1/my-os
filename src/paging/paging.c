#include <stdint.h>
#include "paging.h"
#include "pmm/pmm.h"
#include "vga/vga.h"
#include "user/user.h"

#define PAGE_SIZE 4096

// flags for memory regions
#define PRESENT 0x1
#define WRITABLE 0x2
#define USER 0x4

//array of addresses of page tables (alignment is needed for CPU convention)
static uint32_t page_directory[1024] __attribute__((aligned(4096)));
//array of pointers to page tables (same addresses as page_directory)
static uint32_t* page_tables[1024];

//Enable paging (load CR3 + set PG bit in CR0)
static void enable_paging(uint32_t *page_directory_phys)
{
    //Load page directory base register
    asm volatile("mov %0, %%cr3" :: "r"(page_directory_phys)); // cr3 is the paging directory register

    //read cr0 from cpu
    uint32_t cr0;

    asm volatile("mov %%cr0, %0" : "=r"(cr0)); // cr0 is a flags register, that has a bit dedicated to paging enabling


    //write "enable paging bit" to be 1
    cr0 |= 0x80000000;


    //update new cr0 value
    asm volatile("mov %0, %%cr0" :: "r"(cr0));
}

//VIRTUAL ADDRESS ANATOMY:
//
//31                     22 21                    12 11          0
//+------------------------+------------------------+-------------+
//| Page Directory Index   |   Page Table Index    | Offset      |
//|       10 bits          |       10 bits         |  12 bits     |
//+------------------------+------------------------+-------------+
//
//NOTE: "page directory index" is the page table, "page table index" is the page inside the page table.

void map_page(uint32_t virt, uint32_t phys, uint32_t flags)
{ 
    uint32_t pd = virt >> 22; //page directory index is high 10 bits
    uint32_t pt = (virt >> 12) & 0x3FF; //page table index is middle 10 bits

    //if page table doesn't exist yet, create one
    if (!page_tables[pd])
    {
        uint32_t* table = alloc_page();

        if (!table)
        {
            puts("OUT OF MEMORY\n");
            while(1);
        }

        //clear page table
        for (int i = 0; i < 1024; i++)
        {
            table[i] = 0;
        }

        page_tables[pd] = table;
        page_directory[pd] = ((uint32_t)table) | PRESENT | WRITABLE;
    }

    page_tables[pd][pt] = (phys & 0xFFFFF000) | flags; //put physical address in the correct page index
    if (flags & USER)
        page_directory[pd] |= USER;

    //invalidate cached translation
    asm volatile("invlpg (%0)" :: "r"(virt) : "memory");
}

void unmap_page(uint32_t virt)
{
    uint32_t pd = virt >> 22;
    uint32_t pt = (virt >> 12) & 0x3FF;


    if (!page_tables[pd])
        return;


    page_tables[pd][pt] = 0;


    // invalidate cached translation in CPU
    asm volatile("invlpg (%0)" :: "r"(virt) : "memory");
}


void paging_init(void)
{
    for (int i = 0; i < 1024; i++)
    {
        page_directory[i] = 0;
        page_tables[i] = 0;
    }

    for (uint32_t addr = 0; addr < 32 * 1024 * 1024; addr += PAGE_SIZE)
    {
        map_page(addr, addr, PRESENT | WRITABLE);
    }

    // allow user mode to execute user_test
    map_page((uint32_t)user_test, (uint32_t)user_test, PRESENT | WRITABLE | USER);

    //allow user mode to use its stack
    map_page(0x8F000, 0x8F000,PRESENT | WRITABLE | USER);

    enable_paging(page_directory);
}
