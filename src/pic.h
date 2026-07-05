#pragma once
#include <stdint.h>

// Initialize PIC (remap IRQs to 32–47)
void pic_remap(void);

// Send End Of Interrupt signal
void pic_eoi(uint8_t irq);

void outb(uint16_t port, uint8_t val);

uint8_t inb(uint16_t port);




