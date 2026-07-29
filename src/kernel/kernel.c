#include <kernel/kernel.h>
#include <kernel/task.h>
#include <vga/vga.h>
#include <arch/i386/gdt.h>
#include <timer/timer.h>
#include <user/user.h>
#include <memory/pmm.h>
#include <memory/paging.h>
#include <kernel/scheduler.h>
#include <kernel/fs/ramfs.h>
#include <kernel/fs/vfs.h>
#include <kernel/fs/block.h>
#include <arch/i386/tss.h>
#include <pic/pic.h>
#include <arch/i386/idt.h>
#include <drivers/ata/ata.h>
#include <stdint.h>

void kmain(void)
{

    puts("Booting MYOS...\n");

    //GDT setup
    gdt_init();
    puts("GDT loaded\n");
    
    //TSS setup
    tss_init();
    puts("TSS loaded\n");

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


    //remove mapping
    unmap_page(0x500000);
    puts("unmapped\n");

    //initialize filesystem
    ramfs_init();
    vfs_init();

    //filesystem test
    ramfs_create("hello");
    int fd = vfs_open("hello");
    if(fd >= 0)
    {
        puts("VFS OPEN OK\n");
    }
    else
    {
        puts("VFS OPEN FAIL\n");
    }

    //ATA test
    ata_init();
    puts("ATA initialized\n");
 
    uint8_t buffer[BLOCK_SIZE];

    block_read(1, buffer);
    block_write(1, buffer);

    //enable interrupts
    asm volatile("sti");
    
    //task test
    task_init();
 
    task_create_user(user_test1);
    task_create_user(user_test2);
 
    //idle loop 
    while (1)
    {
        asm volatile("hlt");
    }
}
