#include <drivers/keyboard/keyboard.h>
#include <vga/vga.h>
#include <pic/pic.h>

#define KEYBOARD_BUFFER_SIZE 128

static char buffer[KEYBOARD_BUFFER_SIZE];

static uint32_t write_pos = 0;
static uint32_t read_pos = 0;


static const char kbdus[128] = {
    0,
    27,
    '1','2','3','4','5','6','7','8','9','0','-','=',
    '\b',
    '\t',
    'q','w','e','r','t','y','u','i','o','p','[',']',
    '\n',
    0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,
    '\\',
    'z','x','c','v','b','n','m',',','.','/',
    0,
    '*',
    0,
    ' ',
};


void keyboard_init()
{
    write_pos = 0;
    read_pos = 0;
}

void keyboard_handler()
{
    uint8_t sc = inb(0x60); //read from keyboard controller

    if(sc & 0x80) //ignore key releases (0x80 = bit mask 1000,0000)
    {
        pic_eoi(1);
        return;
    }

    char c = kbdus[sc];

    if(c)
    {
        buffer[write_pos] = c; //store pressed key

        write_pos++;

        if(write_pos >= KEYBOARD_BUFFER_SIZE)
            write_pos = 0;
    }

    pic_eoi(1);
}

int keyboard_read(char* out, uint32_t size)
{
    uint32_t count = 0;

    while(read_pos != write_pos && count < size)
    {
        out[count] = buffer[read_pos]; //write from keyboard buffer into given buffer

        read_pos++;

        if(read_pos >= KEYBOARD_BUFFER_SIZE)
            read_pos = 0;

        count++;
    }

    return count;
}
