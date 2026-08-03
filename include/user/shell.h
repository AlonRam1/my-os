#pragma once

static void shell_parse(char* line, char** command, char** argument);

static void shell_execute(const char* command, const char* argument);

void shell_task();
