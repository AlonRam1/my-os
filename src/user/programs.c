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

    volatile uint32_t value = 123;

    syscall_write_string("MEMTEST: writing value...\n");
    value = 456;

    if(value == 456)
        syscall_write_string("MEMTEST: memory test passed\n");
    else
        syscall_write_string("MEMTEST: memory test failed\n");

    syscall_write_string("MEMTEST: exiting\n");

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
