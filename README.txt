##MYOS

MYOS is a small 32 bit x86 operating system. 

The OS is monolithic, and includes a bootable kernel, protected-mode execution, GDT, TSS, IDT, hardware interrupts,
timer interrupts, keyboard input, physical memory management, paging, page-fault handling, kernel and user-mode execution,
system calls, task scheduling, sleeping and context switching, ATA disk access, and a persistent filesystem.

##SETTING UP MYOS

Build requirements:

GCC/i386 toolchain
GNU Make
GRUB tools
xorriso
QEMU

To build MYOS, execute 'make build' (or simply 'make'). To run the OS from qemu, execute 'make run'. to delete the existing iso image, execute 'make clean'.

##USING MYOS

MYOS greets you with a welcome message and presents you with a terminal screen. From there you can execute the following commands:

#echo <arg>: print <arg> to screen.

#ls: print all files/directories in the current directory.

#clear: clear all text on screen.

#touch <arg>: create new file named <arg>.

#rm <arg>: remove file <arg> from current directory.

#mkdir <arg>: create new directory <arg>.

#cd <arg>: move into directory <arg>.

#run <arg>: run script <arg>.

##RUNNING SCRIPTS

Scripts runnable in the OS are logged in the source file src/user/program.c and are represented by methods.
The scripts given with the OS are:

#memtest: A program that tests the system's virtual memory system. NOTE: the file throws interrupt 14 on purpose to test unmapped memory access.

#scheduler: A program that tests the system's scheduler by calling multiple print functions with sleeps in between, allowing to run multiple scripts at the same time.

#interrupts: A program that tests the system's scheduler by calling yield manually, allowing to inspect the round-robin function of the scheduler.
