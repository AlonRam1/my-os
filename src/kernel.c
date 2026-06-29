void puts(const char* s); 

void kmain(void)
{
    puts("kernel started\n");

    while (1)
        asm volatile("hlt");
}
