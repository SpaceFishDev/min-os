#include "paging.h"
#include "frame.h"
#include "../utils/debug/debug.h"
extern uint32 end_of_kernel;
uint32 placement_address = (uint32)&end_of_kernel;
uint8 enabled = 1;
static uint32 kmalloc_imp(uint32 sz, int align, uint32 *phys)
{
    if (!enabled)
    {
        PANIC("kmalloc is disabled!");
    }
    if (align == 1 && (placement_address & 0xFFFFF000))
    {
        placement_address &= 0xFFFFF000; // Set to current page
        placement_address += 0x1000;     // Advance one page
    }
    if (phys)
    {
        *phys = placement_address;
    }
    uint32 tmp = placement_address;
    placement_address += sz;
    return tmp;
}
uint32 e_kmalloc_a(uint32 sz)
{
    return kmalloc_imp(sz, 1, 0);
}
uint32 e_kmalloc_p(uint32 sz, uint32 *phys)
{
    return kmalloc_imp(sz, 0, phys);
}
uint32 e_kmalloc_ap(uint32 sz, uint32 *phys)
{
    return kmalloc_imp(sz, 1, phys);
}

uint32 e_kmalloc(uint32 sz)
{
    return kmalloc_imp(sz, 0, 0);
}
uint32 disable_early_kmalloc()
{
    uint32 end = placement_address;
    if (!(end & 0xFFF))
        return end;

    end &= 0xFFFFF000; // Set to current page
    end += 0x1000;
    enabled = 0;
    return end;
}

page_directory *kern_dir;
struct page_directory *current_directory;
extern uint32 placement_address;

uint8 initialized = 0;

void identity_map(uint32 address, uint32 length)
{
    uint32 curr = address & 0xFFFFF000;
    while (curr < address + length)
    {
        page *p = get_page(curr, 1, kern_dir);
        p->present = 1;
        p->rw = 1;
        p->user = 0;
        p->frame = curr >> 12;
        curr += 0x1000;
    }
}
#include "../pic/pic.h"
void page_fault()
{
    debug_terminal_writestring("PAGE FAULT!\n");
    while (1)
        ;
}

page *get_page(uint32 address, int make, page_directory *dir)
{
    address /= 0x1000;
    uint32 table_idx = address / 1024;
    if (dir->tables[table_idx])
    {
        return &dir->tables[table_idx]->pages[address % 1024];
    }
    else if (make)
    {
        uint32 tmp;
        if (!initialized)
        {
            dir->tables[table_idx] = (struct page_table *)e_kmalloc_ap(sizeof(page_table), &tmp);
        }
        memset(dir->tables[table_idx], 0, 0x1000);
        dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
        return &dir->tables[table_idx]->pages[address % 1024];
    }
    else
    {
        return 0;
    }
}
#include "../heap/heap.h"

void switch_page_directory(page_directory *dir)
{
    current_directory = dir;
    asm volatile("mov %0, %%cr3" ::"r"(&dir->tablesPhysical));
    uint32 cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0" ::"r"(cr0));
}

void initialize_memory(uint32 total_frames, uint32 ident_addr, uint32 ident_len)
{
    debug_terminal_writestring("Allocating page tables.\n");
    init_frame_allocator(total_frames);
    kern_dir = (struct page_directory *)e_kmalloc_a(sizeof(page_directory));
    memset(kern_dir, 0, sizeof(page_directory));
    current_directory = kern_dir;
    for (unsigned int i = 0; i < 0xFFFFFFFF;)
    {
        get_page(i, 1, kern_dir);
        i += 0x1000 * 1024;
        if (i == 0)
        {
            break;
        }
    }
    int i = 0;
    while (i < placement_address)
    {
        // Kernel code is readable but not writeable from userspace.
        alloc_frame(get_page(i, 1, kern_dir), 0, 0);
        i += 0x1000;
    }
    register_interrupt_handler(14, page_fault);
    uint32 heap_start = disable_early_kmalloc();
    for (i = 0; i < INITIAL_HEAP_PAGE_COUNT; i++)
    {
        alloc_frame(get_page(heap_start + (i * 0x1000), 1, kern_dir), 0, 0);
    }
    if (ident_addr)
    {
        identity_map(ident_addr, ident_len);
    }
    switch_page_directory(kern_dir);
    initialized = 1;
    disable_early_kmalloc();
    init_heap(heap_start);
}

page *map_kernel_page(uint32 address, int make)
{
    page *p = get_page(address, make, kern_dir);
    if (!p)
        return 0;
    alloc_frame(p, 0, 0);
    return p;
}
