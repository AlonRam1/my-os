#include "user.h"
#include "vga/vga.h"

void user_test()
{
    while(1)
    {
        asm volatile("nop");
    }
}
