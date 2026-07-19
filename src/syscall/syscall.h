#pragma once

#include <stdint.h>

#define SYS_WRITE 1
#define SYS_EXIT 2

void syscall_handler(uint32_t* regs);
