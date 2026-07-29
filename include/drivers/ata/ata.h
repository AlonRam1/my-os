#pragma once

#include <stdint.h>

void ata_init();

int ata_read_sector(uint32_t sector, uint8_t* buffer);

int ata_write_sector(uint32_t sector, uint8_t* buffer);
