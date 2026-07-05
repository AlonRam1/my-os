#include "vga.h"
#include "idt.h"
#include "pic.h"
#include "pmm.h"
#include <stdint.h>

/*
 * -----------------------------
 * PAGE TABLES (identity mapping)
 * -----------------------------
 * These must remain alive and aligned.
 * For now we identity-map the first 4MB.
 */

static uint32_t page_directory[1024] __attribute__((aligned(4096)));
static uint32_t page_table0[1024] __attribute__((aligned(4096)));

/*
 * Build identity mapping:
 * virtual address == physical address (0x0 - 0x3FFFFF)
 *
 * This is REQUIRED so that:
 * - kernel code still runs after paging is enabled
 * - stack still works
 * - VGA memory still works
 */
static void paging_init(void)
{
    // Clear structures to avoid garbage mappings
    for (int i = 0; i < 1024; i++)
    {
        page_directory[i] = 0;
        page_table0[i] = 0;
    }

    // Identity map first 4MB (1024 pages * 4KB)
    for (int i = 0; i < 1024; i++)
    {
        page_table0[i] = (i * 0x1000) | 0x3; // present + writable
    }

    // Point first page directory entry to page table
    page_directory[0] = ((uint32_t)page_table0) | 0x3;
}

/*
 * Enable paging in CPU:
 * - CR3 = page directory base address
 * - set PG bit in CR0
 */
static void enable_paging(uint32_t *pd)
{
    asm volatile("mov %0, %%cr3" :: "r"(pd));

    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));

    cr0 |= 0x80000000; // enable paging bit

    asm volatile("mov %0, %%cr0" :: "r"(cr0));
}

/*
 * Kernel entry point
 */
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
     * PAGING SETUP (CRITICAL)
     * -----------------------------
     * MUST happen before:
     * - PMM usage
     * - higher memory access
     * - any non-identity mapped access
     */
    paging_init();
    enable_paging(page_directory);

    puts("Paging enabled\n");

    /*
     * -----------------------------
     * SAFE MEMORY TEST (NOW VALID)
     * -----------------------------
     * This address is identity-mapped.
     */
    volatile int *p = (int*)0x00005000;
    *p = 123;

    puts("Memory write OK\n");

    /*
     * -----------------------------
     * INTERRUPTS TEST (SAFE NOW)
     * -----------------------------
     * Only enable AFTER system is stable.
     */
    asm volatile("sti");

    /*
     * OPTIONAL: PMM (safe after paging)
     * Ensure your PMM does NOT touch unmapped memory yet.
     */
    pmm_init(32 * 1024 * 1024);

    void* a = alloc_page();
    free_page(a);

    puts("PMM test done\n");

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
