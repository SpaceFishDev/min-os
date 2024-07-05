#include "heap.h"
int mem_end = INITIAL_HEAP_PAGE_COUNT * 0x1000;
#define NUM_BLOCK (0x1000 / sizeof(heap_block))
heap_block *base;
int nblock = 1;
char *baseptr;
heap_block *block_dat;
void init_heap(uint32 start_addr)
{
    block_dat = start_addr;
    baseptr = start_addr + 0x1000;
    mem_end = (char *)baseptr;
    base = block_dat;
    base->size = mem_end - start_addr;
    base->ptr = start_addr;
    base->free = true;
}

void create_new_block()
{
    if (nblock > NUM_BLOCK)
    {
        PANIC("TOO MANY BLOCKS?!");
        return;
    }
    block_dat[nblock].size = 0;
    block_dat[nblock].ptr = 0;
    block_dat[nblock].free = true;
    ++nblock;
}

char *malloc(uint32 sz)
{
    for (int i = 0; i < nblock; ++i)
    {
        if (block_dat[i].size >= sz && block_dat[i].free == true)
        {
            if (block_dat[i].size == sz)
            {
                block_dat[i].free = false;
                return block_dat[i].ptr;
            }
            char *ptr = block_dat[i].ptr;
            block_dat[i].ptr += sz;
            block_dat[i].size -= sz;
            create_new_block();
            block_dat[nblock - 1].size = sz;
            block_dat[nblock - 1].ptr = ptr;
            return ptr;
        }
    }
    return 0;
}

void expand()
{
    uint32 size = 0x2000;
    uint32 num_frame = size / 0x2000;
    for (int i = 0; i < num_frame; i++)
    {
        struct page *page = map_kernel_page((uint32)mem_end, 0);
        if (page == 0)
        {
            break;
        }
        mem_end += 0x1000;
    }
}