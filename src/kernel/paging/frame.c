#include "frame.h"
uint32 stack_count = 0; // The current capacity of the stack
uint32 *free_frames = 0;
int32 top_of_stack = -1;

// If the stack is empty, we allocate from the end of physical memory.
// Since we've not allocated any pages, end_of_mem begins at 0.
uint32 end_of_mem = 0;

uint32 allocated_frames = 0;
uint32 total_frames;

void init_frame_allocator(uint32 system_frames)
{
    total_frames = system_frames;
    if (free_frames != 0)
    {
        // We've already initialized the frame allocator!
        return;
    }

    // Allocate a big enough stack to hold all the frames on the system.
    // Should be at most 4 MiB.
    free_frames = (uint32 *)e_kmalloc(system_frames * sizeof(uint32));
    stack_count = system_frames;
}

void alloc_frame(page *page, int is_kernel, int is_writeable)
{
    if (page->frame != 0)
    {
        return; // Frame was already allocated, return straight away.
    }
    allocated_frames++;
    uint32 idx;
    if (top_of_stack >= 0)
    {
        // There are free frames in the stack!
        idx = free_frames[top_of_stack];
        top_of_stack--; // POP
    }
    else
    {
        if (end_of_mem >= total_frames)
        {
            PANIC("Cannot alloc frame. Out of memory!");
        }
        idx = end_of_mem;
        end_of_mem++;
    }
    page->present = 1;                 // Mark it as present.
    page->rw = (is_writeable) ? 1 : 0; // Should the page be writeable?
    page->user = (is_kernel) ? 0 : 1;  // Should the page be user-mode?
    page->frame = idx;
}

void free_frame(page *page)
{
    top_of_stack++;
    if (((uint16)top_of_stack) >= stack_count)
    {
        PANIC("Frame pool is full! Something weird happened!");
    }

    free_frames[top_of_stack] = page->frame;
    page->frame = 0x0;
    allocated_frames--;
}