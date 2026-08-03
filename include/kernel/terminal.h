#pragma once

#include <stdint.h>

void terminal_init();

int terminal_read(char* buffer, uint32_t size);

int terminal_readline(char* buffer, uint32_t size);

void terminal_write(const char* buffer);
