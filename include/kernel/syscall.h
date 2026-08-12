#pragma once

#include <stdint.h>

#define SYS_EXIT 0
#define SYS_WRITE 1
#define SYS_YIELD 2
#define SYS_SLEEP 3
#define SYS_READ 4
#define SYS_WRITE_STRING 5
#define SYS_CREATE_FILE 6
#define SYS_DELETE_FILE 7
#define SYS_CREATE_DIRECTORY 8

void syscall_handler(uint32_t* regs);
