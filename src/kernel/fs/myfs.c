#include <kernel/fs/myfs.h>
#include <kernel/fs/block.h>

static struct myfs_superblock superblock;

static struct myfs_inode inodes[MYFS_MAX_FILES];


//initialize a new filesystem on disk
void myfs_format(uint32_t total_blocks)
{
    superblock.magic = MYFS_MAGIC;
    superblock.total_blocks = total_blocks;
    superblock.free_blocks = total_blocks - MYFS_DATA_START;


    //write superblock to sector 0
    block_write(0, (uint8_t*)&superblock);


    //clear inode table
    for(int i = 0; i < MYFS_MAX_FILES; i++)
    {
        inodes[i].used = 0;
        inodes[i].size = 0;
        inodes[i].block = 0;
    }


    //write inode table to disk
    for(int i = 0; i < 8; i++)
    {
        block_write(
            MYFS_INODE_START + i,
            ((uint8_t*)inodes) + (i * MYFS_BLOCK_SIZE)
        );
    }
}

//load filesystem metadata from disk
int myfs_mount()
{
    //read superblock
    block_read(0, (uint8_t*)&superblock);


    //check filesystem magic number
    if(superblock.magic != MYFS_MAGIC)
    {
        return -1;
    }


    //read inode table
    for(int i = 0; i < 8; i++)
    {
        block_read(
            MYFS_INODE_START + i,
            ((uint8_t*)inodes) + (i * MYFS_BLOCK_SIZE)
        );
    }


    return 0;
}
