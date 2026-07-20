#pragma once
#include <stdint.h>

#define TIME_QUANTUM 5 //how many ticks before task switch
extern uint32_t time_slice; //which tick (modulu TIME_QUANTUM) we are on

uint32_t schedule(uint32_t* esp);
