#pragma once

#include <stdint.h>

void keyboard_init();

void keyboard_handler();

int keyboard_read(char* buffer, uint32_t size);
