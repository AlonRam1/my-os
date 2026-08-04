#pragma once


struct command
{
    char* name;
    void (*execute)(const char* args);
};

void execute(char* name, char* args);


