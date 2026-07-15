// src/isr.h
#pragma once
#include <stdint.h>

void isr0();
void isr1();
void isr3();
void isr4();
void isr5();
void isr6();
void isr8();
void isr13();
void isr14();
void irq0();
void irq1();

void isr0_handler();
void isr1_handler();
void isr3_handler();
void isr4_handler();
void isr5_handler();
void isr6_handler();
void isr8_handler();
void isr13_handler();
void isr14_handler();
void irq0_handler(uint32_t* esp);
void irq1_handler();
