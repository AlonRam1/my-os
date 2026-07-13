#include "vga/vga.h"
#include "idt/idt.h"
#include "pic/pic.h"
#include "pmm/pmm.h"
#include "paging/paging.h"
#include <stdint.h>


void kmain(void)
{
    /*
     * -----------------------------
     * BASIC OUTPUT FIRST
     * -----------------------------
     * VGA is identity mapped in low memory,
     * so this is safe BEFORE paging.
     */
    puts("Booting MYOS...\n");


    /*
     * -----------------------------
     * IDT + PIC SETUP
     * -----------------------------
     * Interrupt system must be initialized
     * before enabling interrupts (sti).
     */
    idt_init();
    puts("IDT loaded\n");

    pic_remap();
    puts("PIC remapped\n");


    /*
     * -----------------------------
     * PMM SETUP
     * -----------------------------
     * Paging needs physical pages
     * to create page tables.
     */
    pmm_init(32 * 1024 * 1024);
    puts("PMM initialized\n");


    /*
     * -----------------------------
     * PAGING SETUP (CRITICAL)
     * -----------------------------
     * MUST happen before:
     * - higher memory access
     * - virtual memory mappings
     * - non-identity mapped access
     */
    paging_init();

    puts("Paging enabled\n");


    /*
     * -----------------------------
     * SAFE MEMORY TEST
     * -----------------------------
     * This address is identity-mapped.
     */
    volatile int *p = (int*)0x00005000;
    *p = 123;

    puts("identity write OK\n");


    /*
     * -----------------------------
     * VIRTUAL MEMORY TEST
     * -----------------------------
     * Allocate physical memory,
     * map it to a virtual address,
     * then access it.
     */
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


    /*
     * -----------------------------
     * PAGE FAULT TEST
     * -----------------------------
     * This should trigger INT 14
     * because the mapping was removed.
     */
    *x = 456;


    puts("this should NOT print\n");


    /*
     * -----------------------------
     * INTERRUPTS ENABLE
     * -----------------------------
     */
    asm volatile("sti");


    /*
     * -----------------------------
     * IDLE LOOP
     * -----------------------------
     */
    while (1)
    {
        asm volatile("hlt");
    }
}
