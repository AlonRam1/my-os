#include <stdint.h>
// vga.h
void puts(const char* s);
void putchar(char c);
void puthex(uint8_t v);
void copyline(uint8_t oldrow, uint8_t newrow);
void clearline(uint8_t r);
void clearscreen();
void scrollup();
