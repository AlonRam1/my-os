#include "vga.h"
#include "idt.h"
#include "pic.h"
#include "pmm.h"
#include <stdint.h>

static uint32_t page_directory[1024] __attribute__((aligned(4096)));
static uint32_t page_table0[1024] __attribute__((aligned(4096)));

static void paging_init(void)
{
    // clear page directory and table
    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0;
        page_table0[i] = 0;
    }

    // identity map first 4MB
    for (int i = 0; i < 1024; i++) {
        page_table0[i] = (i * 0x1000) | 3;  
        // 0x3 = present + writable
    }

    // point directory entry 0 to page table
    page_directory[0] = ((uint32_t)page_table0) | 3;
}

static void enable_paging(uint32_t *pd)
{
    asm volatile("mov %0, %%cr3" :: "r"(pd));

    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));

    cr0 |= 0x80000000; // set PG bit

    asm volatile("mov %0, %%cr0" :: "r"(cr0));
}

void kmain(void)
{
    volatile uint16_t cs;
    puts("WHAT'S GOOD NIGGA?? WELCOME TO MYOS\n");
    
    idt_init(); //load IDT
    puts("IDT loaded\n");
    pic_remap();

    asm volatile("int $0x0"); //REMOVE: INT0 TEST
    asm volatile("int $0x1"); 
    asm volatile("int $0x3");
    asm volatile("int $0x4");
    asm volatile("int $0x5");
    asm volatile("int $0x6");
    asm volatile("int $0x8");

    //int 13
    asm volatile("mov $0x23, %eax");  
    asm volatile("mov %eax, %ds");    

    paging_init();
    enable_paging(page_directory);

    puts("paging enabled\n");

    //int 14
    volatile int *p = (int*)0x500000; // not mapped (above 4MB)
    *p = 123;

    puts("AFTER INT\n");
    asm volatile("sti");

     pmm_init(32 * 1024 * 1024); // pretend 32MB

    void* a = alloc_page();
    void* b = alloc_page();

    free_page(a);

    //enter idle loop until next interrupt
    while (1)
        asm volatile("hlt");
}
