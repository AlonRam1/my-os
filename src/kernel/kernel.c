#include "vga/vga.h"
#include "idt/idt.h"
#include "pic/pic.h"
#include "pmm/pmm.h"
#include "task/task.h"
#include "paging/paging.h"
#include <stdint.h>

void task_a()
{
    while(1)
    {
        puts("A\n");
        task_yield();
    }
}

void task_b()
{
    while(1)
    {
        puts("B\n");
        task_yield();
    }
}

void kmain(void)
{

    puts("Booting MYOS...\n");

    //IDT and PIC setup
    idt_init();
    puts("IDT loaded\n");

    pic_remap();
    puts("PIC remapped\n");


    //pmm setup
    pmm_init(32 * 1024 * 1024);
    puts("PMM initialized\n");

    //paging setup
    paging_init();

    puts("Paging enabled\n");

    //identity write test
    volatile int *p = (int*)0x00005000;
    *p = 123;

    puts("identity write OK\n");

    //VM test
    void* phys = alloc_page();

    map_page(0x500000, (uint32_t)phys, 0x3);

    puts("mapped\n");


    // write to virtual address
    volatile int* x = (int*)0x500000;
    *x = 123;

    puts("write OK\n");


    // remove mapping
    unmap_page(0x500000);

    puts("unmapped\n");

    //page fault test
    *x = 456;

    //enable interrupts
    asm volatile("sti");

    //task test
    task_init();

    task_create(task_a);
    task_create(task_b);

    while(1)
    {
	task_yield();
    }


    //idle loop 
    while (1)
    {
        asm volatile("hlt");
    }
}


