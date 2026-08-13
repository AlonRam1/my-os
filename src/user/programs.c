#include <user/programs.h>
#include <user/user.h>
#include <kernel/string/string.h>
#include <kernel/task.h>
#include <stdint.h>

static void memtest_program();
static void scheduler_program();
static void interrupt_program();

struct user_program
{
    const char* name;
    void (*entry)();
};

static const struct user_program programs[] =
{
    { "memtest", memtest_program },
    { "scheduler", scheduler_program },
    { "interrupts", interrupt_program }
};

static const int program_count = sizeof(programs) / sizeof(programs[0]);

static void memtest_program()
{
    syscall_write_string("MEMTEST: started\n");

    //test 1: stack memory
    syscall_write_string("MEMTEST: testing stack memory...\n");

    volatile uint32_t values[4];

    values[0] = 0x12345678;
    values[1] = 0xDEADBEEF;
    values[2] = 0xAAAAAAAA;
    values[3] = 0x55555555;

    if(values[0] != 0x12345678 || values[1] != 0xDEADBEEF || values[2] != 0xAAAAAAAA || values[3] != 0x55555555)
    {
        syscall_write_string("MEMTEST: stack memory FAILED\n");
        syscall_exit();
    }

    syscall_write_string("MEMTEST: stack memory passed\n");

    //test 2: repeated writes and reads
    syscall_write_string("MEMTEST: testing repeated memory access...\n");

    volatile uint32_t value = 0;

    for(uint32_t i = 0; i < 1000; i++)
    {
        value = i;

        if(value != i)
        {
            syscall_write_string("MEMTEST: repeated access FAILED\n");
            syscall_exit();
        }
    }

    syscall_write_string("MEMTEST: repeated memory access passed\n");

    //test 3: page fault
    syscall_write_string("MEMTEST: testing page fault handling...\n");
    syscall_write_string("MEMTEST: accessing unmapped memory...\n");

    volatile uint32_t* bad_address = (uint32_t*)0x40000000;
    *bad_address = 123;

    //this should never execute if page fault handling works
    syscall_write_string("MEMTEST: page fault FAILED\n");

    syscall_exit();
}

static void scheduler_program()
{
    syscall_write_string("SCHEDULER: started\n");

    syscall_write_string("SCHEDULER: step 1\n");
    syscall_sleep(1000000);

    syscall_write_string("SCHEDULER: step 2\n");
    syscall_sleep(1000000);

    syscall_write_string("SCHEDULER: step 3\n");
    syscall_sleep(1000000);

    syscall_write_string("SCHEDULER: step 4\n");
    syscall_sleep(1000000);

    syscall_write_string("SCHEDULER: step 5\n");
    syscall_sleep(1000000);

    syscall_write_string("SCHEDULER: finished\n");

    syscall_exit();
}

static void interrupt_program()
{
    syscall_write_string("INTERRUPT: started\n");

    syscall_write_string("INTERRUPT: yielding to scheduler\n");
    syscall_yield();

    syscall_write_string("INTERRUPT: returned from yield\n");

    syscall_write_string("INTERRUPT: exiting\n");

    syscall_exit();
}

int user_program_run(const char* name)
{
    for(int i = 0; i < program_count; i++)
    {
        if(streq(programs[i].name, name))
        {
            return task_create_user(programs[i].entry) < 0 ? -1 : 0;
        }
    }

    return -1;
}
