#pragma once

#include <stdint.h>

void syscall_write(char c);
void syscall_write_string(const char* string);
void syscall_exit();
void syscall_yield();
void syscall_sleep(uint32_t ticks);

int syscall_create_file(const char* name);
int syscall_delete_file(const char* name);
int syscall_create_directory(const char* name);

void user_exit_stub();
