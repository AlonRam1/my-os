#include <kernel/fs/myfs.h>
#include <kernel/fs/block.h>
#include <kernel/string/string.h>

static struct myfs_superblock superblock;

static struct myfs_inode inodes[MYFS_MAX_FILES];


//write inode table to disk
static void myfs_write_inodes()
{
    for(int i = 0; i < 8; i++)
    {
        block_write(MYFS_INODE_START + i, ((uint8_t*)inodes) + (i * MYFS_BLOCK_SIZE));
    }
}


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
    myfs_write_inodes();
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
        block_read(MYFS_INODE_START + i, ((uint8_t*)inodes) + (i * MYFS_BLOCK_SIZE));
    }

    return 0;
}


//create a new file
int myfs_create(const char* name)
{
    //check if file already exists
    for(int i = 0; i < MYFS_MAX_FILES; i++)
    {
        if(inodes[i].used)
        {
            if(streq(inodes[i].name, name))
                return -1;
        }
    }

    //find free inode
    for(int i = 0; i < MYFS_MAX_FILES; i++)
    {
        if(!inodes[i].used)
        {
            inodes[i].used = 1;
            inodes[i].size = 0;

            //one data block per file for now
            inodes[i].block = MYFS_DATA_START + i;

            strcopy(inodes[i].name, name);

            //save inode table
            myfs_write_inodes();

            return 0;
        }
    }

    return -1;
}


//find file inode by name
struct myfs_inode* myfs_find(const char* name)
{
    for(int i = 0; i < MYFS_MAX_FILES; i++)
    {
        if(inodes[i].used)
        {
            if(streq(inodes[i].name, name))
                return &inodes[i];
        }
    }

    return 0;
}


//write file data
int myfs_write(struct myfs_inode* inode, const uint8_t* buffer, uint32_t size)
{
    if(!inode)
        return -1;

    if(size > MYFS_BLOCK_SIZE)
        size = MYFS_BLOCK_SIZE;

    uint8_t block[MYFS_BLOCK_SIZE];

    for(int i = 0; i < MYFS_BLOCK_SIZE; i++)
    {
        block[i] = 0;
    }

    for(uint32_t i = 0; i < size; i++)
    {
        block[i] = buffer[i];
    }

    block_write(inode->block, block);

    inode->size = size;

    myfs_write_inodes();

    return size;
}


//read file data
int myfs_read(struct myfs_inode* inode, uint8_t* buffer, uint32_t size)
{
    if(!inode)
        return -1;

    if(size > inode->size)
        size = inode->size;

    uint8_t block[MYFS_BLOCK_SIZE];

    block_read(inode->block, block);

    for(uint32_t i = 0; i < size; i++)
    {
        buffer[i] = block[i];
    }

    return size;
}

//find file by index
struct myfs_inode* myfs_inode(int index)
{
    if(index < 0 || index >= MYFS_MAX_FILES)
        return 0;

    return &inodes[index];
}

//delete file
int myfs_delete(const char* name)
{
    for(int i = 0; i < MYFS_MAX_FILES; i++)
    {
        if(inodes[i].used)
        {
            if(streq(inodes[i].name, name))
            {
                inodes[i].used = 0;
                inodes[i].size = 0;
                inodes[i].block = 0;

                for(int j = 0; j < MYFS_NAME_LEN; j++)
                {
                    inodes[i].name[j] = 0;
                }

                myfs_write_inodes();

                return 0;
            }
        }
    }

    return -1;
}
