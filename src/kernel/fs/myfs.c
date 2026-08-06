#include <kernel/fs/myfs.h>
#include <kernel/fs/block.h>
#include <kernel/string/string.h>

static struct myfs_superblock superblock;

static struct myfs_inode inodes[MYFS_MAX_FILES];

uint32_t current_directory = 0;

static void myfs_clear_block(uint32_t block)
{
    uint8_t buffer[MYFS_BLOCK_SIZE];

    for(int i = 0; i < MYFS_BLOCK_SIZE; i++)
    {
        buffer[i] = 0;
    }

    block_write(block, buffer);
}

//write inode table to disk
static void myfs_write_inodes()
{
    for(int i = 0; i < MYFS_DATA_START - 1; i++)
    {
        block_write(MYFS_INODE_START + i, ((uint8_t*)inodes) + (i * MYFS_BLOCK_SIZE));
    }
}

//initialize a new filesystem on disk
void myfs_format(uint32_t total_blocks)
{
    superblock.magic = MYFS_MAGIC; //MYFS identifier for superblock
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

    //create root directory
    inodes[0].used = 1;
    inodes[0].size = 0;
    inodes[0].block = MYFS_DATA_START; //reminder: block 0 is super block, blocks 1-MYFS_DATA_START store inodes (metadata)
    inodes[0].parent = 0; //root dir has no parent
    inodes[0].type = MYFS_DIRECTORY;
    strcopy(inodes[0].name, "/");

//initialize root directory block
myfs_clear_block(inodes[0].block);


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

    //write inodes into inode table
    for(int i = 0; i < MYFS_DATA_START - 1; i++)
    {
        block_read(MYFS_INODE_START + i, ((uint8_t*)inodes) + (i * MYFS_BLOCK_SIZE));
    }

    return 0;
}

//create a new file
int myfs_create_file(const char* name, uint32_t parent)
{
    //check if file already exists
    for(int i = 0; i < MYFS_MAX_FILES; i++)
    {
        if(inodes[i].used)
        {
            if(streq(inodes[i].name, name) && inodes[i].parent == parent)
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

            inodes[i].parent = parent;
            inodes[i].type = MYFS_FILE;

            strcopy(inodes[i].name, name);

            //save inode table
            myfs_write_inodes();
	    
            //add file to parent directory
            myfs_add_directory_entry(parent, i, name);

            return 0;
        }
    }

    return -1;
}

//find file inode by name
struct myfs_inode* myfs_find(const char* name)
{
    struct myfs_inode* current = myfs_inode(current_directory);

    if(!current)
        return 0;

    uint8_t block[MYFS_BLOCK_SIZE];

    block_read(current->block, block);

    struct myfs_dir_entry* entries = (struct myfs_dir_entry*)block;

    for(int i = 0; i < MYFS_BLOCK_SIZE / sizeof(struct myfs_dir_entry); i++)
    {
        if(entries[i].inode != 0)
        {
            struct myfs_inode* inode = myfs_inode(entries[i].inode);

            if(inode && streq(inode->name, name))
            {
                return inode;
            }
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
                uint32_t parent = inodes[i].parent;

                inodes[i].used = 0;
                inodes[i].size = 0;
                inodes[i].block = 0;

                for(int j = 0; j < MYFS_NAME_LEN; j++)
                {
                    inodes[i].name[j] = 0;
                }

                myfs_remove_directory_entry(parent, name);

                myfs_write_inodes();

                return 0;
            }
        }
    }

    return -1;
}
//create a new directory
int myfs_create_directory(const char* name, uint32_t parent)
{
    //check if directory already exists
    for(int i = 0; i < MYFS_MAX_FILES; i++)
    {
        if(inodes[i].used)
        {
            if(streq(inodes[i].name, name) && inodes[i].parent == parent)
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

            inodes[i].block = MYFS_DATA_START + i;

            inodes[i].parent = parent;
            inodes[i].type = MYFS_DIRECTORY;

            strcopy(inodes[i].name, name);

            //initialize empty directory block
            myfs_clear_block(inodes[i].block);

            //save inode table
            myfs_write_inodes();

            //add directory to parent directory
            myfs_add_directory_entry(parent, i, name);

            return 0;
        }
    }

    return -1;
}

int myfs_add_directory_entry(uint32_t directory, uint32_t inode, const char* name)
{
    uint8_t block[MYFS_BLOCK_SIZE];

    block_read(inodes[directory].block, block);

    struct myfs_dir_entry* entries = (struct myfs_dir_entry*)block;

    for(int i = 0; i < MYFS_BLOCK_SIZE / sizeof(struct myfs_dir_entry); i++)
    {
        if(entries[i].inode == 0)
        {
            entries[i].inode = inode;
            strcopy(entries[i].name, name);

            block_write(inodes[directory].block, block);

            return 0;
        }
    }

    return -1;
}

//find file inside a directory
struct myfs_inode* myfs_find_in_directory(uint32_t parent, const char* name)
{
    for(int i = 0; i < MYFS_MAX_FILES; i++)
    {
        if(inodes[i].used)
        {
            if(inodes[i].parent == parent && streq(inodes[i].name, name))
            {
                return &inodes[i];
            }
        }
    }

    return 0;
}

int myfs_remove_directory_entry(uint32_t directory, const char* name)
{
    uint8_t block[MYFS_BLOCK_SIZE];

    block_read(inodes[directory].block, block);

    struct myfs_dir_entry* entries = (struct myfs_dir_entry*)block;

    for(int i = 0; i < MYFS_BLOCK_SIZE / sizeof(struct myfs_dir_entry); i++)
    {
        if(entries[i].inode != 0)
        {
            if(streq(entries[i].name, name))
            {
                entries[i].inode = 0;

                for(int j = 0; j < MYFS_NAME_LEN; j++)
                {
                    entries[i].name[j] = 0;
                }

                block_write(inodes[directory].block, block);

                return 0;
            }
        }
    }

    return -1;
}
