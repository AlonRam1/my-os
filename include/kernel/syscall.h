#pragma once

#include <stdint.h>

#define SYS_EXIT  0
#define SYS_WRITE 1
#define SYS_YIELD 2
#define SYS_SLEEP 3
#define SYS_READ 4

void syscall_handler(uint32_t* regs);
