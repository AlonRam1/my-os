#include <drivers/ata/ata.h>
#include <stdint.h>

#define ATA_DATA        0x1F0
#define ATA_ERROR       0x1F1
#define ATA_SECTOR_CNT  0x1F2
#define ATA_LBA_LOW     0x1F3
#define ATA_LBA_MID     0x1F4
#define ATA_LBA_HIGH    0x1F5
#define ATA_DRIVE       0x1F6
#define ATA_COMMAND     0x1F7
#define ATA_STATUS      0x1F7
#define ATA_DEVICE_CONTROL 0x3F6

#define ATA_CMD_READ    0x20
#define ATA_CMD_WRITE   0x30

#define ATA_STATUS_BUSY 0x80
#define ATA_STATUS_DRQ  0x08

//read from IO port
static inline __attribute__((always_inline)) uint8_t inb(uint16_t port)
{
    uint8_t value;
    asm volatile("inb %%dx, %%al" : "=a"(value) : "d"(port) : "memory");
    return value;
}

//write to IO port
static inline __attribute__((always_inline)) void outb(uint16_t port, uint8_t value)
{
    asm volatile("outb %%al, %%dx" : : "a"(value), "d"(port) : "memory");
}

void ata_init()
{
	outb(ATA_DEVICE_CONTROL, 0x02);
}

static void ata_wait()
{
    while(inb(ATA_STATUS) & ATA_STATUS_BUSY);
    
    while(!(inb(ATA_STATUS) & ATA_STATUS_DRQ));
}

//read 512 bytes from disk
int ata_read_sector(uint32_t sector, uint8_t* buffer)
{
    outb(ATA_DRIVE, 0xE0 | ((sector >> 24) & 0x0F));

    outb(ATA_SECTOR_CNT, 1);

    outb(ATA_LBA_LOW, sector & 0xFF);
    outb(ATA_LBA_MID, (sector >> 8) & 0xFF);
    outb(ATA_LBA_HIGH, (sector >> 16) & 0xFF);

    outb(ATA_COMMAND, ATA_CMD_READ);

    ata_wait();

    for(int i = 0; i < 256; i++)
    {
        uint16_t data;

        asm volatile("inw %%dx, %%ax" : "=a"(data) : "d"(ATA_DATA));

        buffer[i*2] = data & 0xFF;
        buffer[i*2+1] = data >> 8;
    }

    return 0;
}

//write 512 bytes to disk
int ata_write_sector(uint32_t sector, uint8_t* buffer)
{
    outb(ATA_DRIVE, 0xE0 | ((sector >> 24) & 0x0F));

    outb(ATA_SECTOR_CNT, 1);

    outb(ATA_LBA_LOW, sector & 0xFF);
    outb(ATA_LBA_MID, (sector >> 8) & 0xFF);
    outb(ATA_LBA_HIGH, (sector >> 16) & 0xFF);

    outb(ATA_COMMAND, ATA_CMD_WRITE);

    ata_wait();

    for(int i = 0; i < 256; i++)
    {
        uint16_t data = buffer[i*2] | (buffer[i*2+1] << 8);

        asm volatile("outw %%ax, %%dx" : : "a"(data), "d"(ATA_DATA));
    }

    while(inb(ATA_STATUS) & ATA_STATUS_BUSY);

    return 0;
}
