#pragma once

#include <stdint.h>

struct gdt_entry
{
    uint16_t limit_low; //which address space can this segment access?
    uint16_t base_low; //entry address - low
    uint8_t base_middle; //entry address - middle
    uint8_t access; //flags
    uint8_t granularity; //unit for measuring limit (bytes/pages). this field also has limit_high in it
    uint8_t base_high; //entry address - high
} __attribute__((packed));

struct gdt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void gdt_init();
static void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
static void gdt_set_tss(int num, uint32_t base, uint32_t limit);


