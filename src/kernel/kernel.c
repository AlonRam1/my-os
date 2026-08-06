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
#include <kernel/fs/myfs.h>
#include <arch/i386/tss.h>
#include <pic/pic.h>
#include <arch/i386/idt.h>
#include <drivers/ata/ata.h>
#include <drivers/keyboard/keyboard.h>
#include <user/shell.h>
#include <kernel/terminal.h>
#include <stdint.h>

void kmain(void)
{
    puts("Booting MYOS...\n");

    //GDT setup
    gdt_init();

    //TSS setup
    tss_init();

    //IDT and PIC setup
    idt_init();
    pic_remap();

    //pmm setup
    pmm_init(32 * 1024 * 1024);

    //paging setup
    paging_init();

    //identity write test
    volatile int *p = (int*)0x00005000;
    *p = 123;

    //VM test
    void* phys = alloc_page();
    map_page(0x500000, (uint32_t)phys, 0x3);

    //write to virtual address test
    volatile int* x = (int*)0x500000;
    *x = 123;

    //remove mapping test
    unmap_page(0x500000);

    //initialize filesystem
    vfs_init();

    //initialize ATA
    ata_init();

    //initialize MYFS
    if(myfs_mount() == 0)
   {
        puts("MYFS mounted\n");
    }
    else
    {
        puts("MYFS not found, formatting\n");

        myfs_format(16);

        if(myfs_mount() == 0)
        {
            puts("MYFS mounted after format\n");
        }
        else
        {
            puts("MYFS mount failed\n");
        }
    }    
    //initialize keyboard
    keyboard_init();

    //initialize terminal
    terminal_init();

    //scheduler setup
    task_init();
    task_create(shell_task);

    task_create_user(user_test1);
    task_create_user(user_test2);

    //enable interrupts LAST
    asm volatile("sti");


    //idle loop
    while(1)
    {
        asm volatile("hlt");
    }
}
