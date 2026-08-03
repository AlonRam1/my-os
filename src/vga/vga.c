#include <stdint.h>

volatile uint16_t* vga = (uint16_t*)0xB8000;

#define TERMINALHEIGHT 14
#define ROWS 25
#define COLS 80

static int row = 0;
static int col = 0;

void clearline(uint8_t r)
{
    if(r >= ROWS)
	return;
    for(int i = 0; i < COLS ; i++)
    {
	vga[r * COLS + i] = ((uint16_t)0x07 << 8) | ' ';
    }
}

void clearscreen()
{
	for(int i = 0 ; i < ROWS ; i++)
	{
		clearline(i);
	}
	row = 0;
	col = 0;
}

void copyline(uint8_t oldrow, uint8_t newrow)
{
    if(oldrow >= ROWS || newrow >= ROWS)
	return;

    for(int i = 0; i < COLS ; i++)
    {
	vga[newrow * COLS + i] = vga[oldrow * COLS + i];
    }

}

void scrollup()
{
	for(int i = 1; i < TERMINALHEIGHT ; i++)
	{
		copyline(i, i-1);
	}
	clearline(TERMINALHEIGHT - 1);
	row = TERMINALHEIGHT - 1;
	col = 0;
}

void putchar(char c)
{
    asm volatile("cli");

    if (c == '\n')
    {
        row++;
        col = 0;
    }
    else
    {
        vga[row * COLS + col] = (0x0F << 8) | c;
        col++;

        if (col >= COLS)
        {
            col = 0;
            row++;
        }
    }

    if (row >= TERMINALHEIGHT)
    {
        scrollup();
    }

    asm volatile("sti");
}

void puts(const char* s)
{
    while (*s)
    {
        char c = *s++;
        putchar(c); 
    }
}

const char hex[] = "0123456789ABCDEF";

void puthex(uint8_t v)
{
    char high = hex[(v >> 4) & 0xF];
    char low  = hex[v & 0xF];

    putchar(high);
    putchar(low);
}


