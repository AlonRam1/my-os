#include <kernel/fs/block.h>
#include <drivers/ata/ata.h>
#include <vga/vga.h>

//read one block from storage
int block_read(uint32_t block, uint8_t* buffer)
{
    return ata_read_sector(block, buffer);
}
    

//write one block to storage
int block_write(uint32_t block, uint8_t* buffer)
{
    return ata_write_sector(block, buffer);
}
