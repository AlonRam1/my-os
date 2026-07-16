#pragma once

#include <stdint.h>

extern volatile uint32_t ticks;

void timer_tick();
