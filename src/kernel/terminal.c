#include <kernel/terminal.h>
#include <drivers/keyboard/keyboard.h>
#include <kernel/syscall.h>
#include <vga/vga.h>


void terminal_init()
{
}


int terminal_read(char* buffer, uint32_t size)
{
    return keyboard_read(buffer, size);
}

int terminal_readline(char* buffer, uint32_t size)
{
    uint32_t count = 0;

    while(1)
    {
        char c;

        if(keyboard_read(&c, 1) == 0)
        {
            asm volatile("int $0x80" : : "a"(SYS_YIELD));
            continue;
        }


        if(c == '\n')
        {
            buffer[count] = 0;
            return count;
        }


        if(c == '\b')
        {
            if(count > 0)
            {
                count--;

                putchar('\b');
            }

            continue;
        }


        if(count < size - 1)
        {
            buffer[count] = c;
            count++;

            putchar(c);
        }
    }
}

void terminal_write(const char* buffer)
{
    puts(buffer);
}
