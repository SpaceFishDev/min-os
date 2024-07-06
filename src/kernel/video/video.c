#include "video.h"
#include "../utils/util.h"
#include "../utils/debug/debug.h"
#include "../heap/heap.h"
#include "../pit/pit.h"
#include "../shell/font.h"
uint8 *video_buffer = 0xA0000;

#define VGA_AC_INDEX 0x3C0
#define VGA_AC_WRITE 0x3C0
#define VGA_AC_READ 0x3C1
#define VGA_MISC_WRITE 0x3C2
#define VGA_SEQ_INDEX 0x3C4
#define VGA_SEQ_DATA 0x3C5
#define VGA_DAC_READ_INDEX 0x3C7
#define VGA_DAC_WRITE_INDEX 0x3C8
#define VGA_DAC_DATA 0x3C9
#define VGA_MISC_READ 0x3CC
#define VGA_GC_INDEX 0x3CE
#define VGA_GC_DATA 0x3CF
#define VGA_CRTC_INDEX 0x3D4
#define VGA_CRTC_DATA 0x3D5
#define VGA_INSTAT_READ 0x3DA

void set_vga_mode_13h()
{
    // Disable the NMI
    uint8 port_value = inb(0x3DA);
    outb(VGA_MISC_WRITE, 0x63);

    // Sequence Controller Registers
    outb(VGA_SEQ_INDEX, 0x00);
    outb(VGA_SEQ_DATA, 0x03);
    outb(VGA_SEQ_INDEX, 0x01);
    outb(VGA_SEQ_DATA, 0x01);
    outb(VGA_SEQ_INDEX, 0x02);
    outb(VGA_SEQ_DATA, 0x0F);
    outb(VGA_SEQ_INDEX, 0x03);
    outb(VGA_SEQ_DATA, 0x00);
    outb(VGA_SEQ_INDEX, 0x04);
    outb(VGA_SEQ_DATA, 0x0E);

    // CRTC Controller Registers
    outb(VGA_CRTC_INDEX, 0x11);
    outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & 0x7F);
    outb(VGA_CRTC_INDEX, 0x00);
    outb(VGA_CRTC_DATA, 0x5F);
    outb(VGA_CRTC_INDEX, 0x01);
    outb(VGA_CRTC_DATA, 0x4F);
    outb(VGA_CRTC_INDEX, 0x02);
    outb(VGA_CRTC_DATA, 0x50);
    outb(VGA_CRTC_INDEX, 0x03);
    outb(VGA_CRTC_DATA, 0x82);
    outb(VGA_CRTC_INDEX, 0x04);
    outb(VGA_CRTC_DATA, 0x54);
    outb(VGA_CRTC_INDEX, 0x05);
    outb(VGA_CRTC_DATA, 0x80);
    outb(VGA_CRTC_INDEX, 0x06);
    outb(VGA_CRTC_DATA, 0xBF);
    outb(VGA_CRTC_INDEX, 0x07);
    outb(VGA_CRTC_DATA, 0x1F);
    outb(VGA_CRTC_INDEX, 0x08);
    outb(VGA_CRTC_DATA, 0x00);
    outb(VGA_CRTC_INDEX, 0x09);
    outb(VGA_CRTC_DATA, 0x41);
    outb(VGA_CRTC_INDEX, 0x10);
    outb(VGA_CRTC_DATA, 0x9C);
    outb(VGA_CRTC_INDEX, 0x11);
    outb(VGA_CRTC_DATA, 0x8E);
    outb(VGA_CRTC_INDEX, 0x12);
    outb(VGA_CRTC_DATA, 0x8F);
    outb(VGA_CRTC_INDEX, 0x13);
    outb(VGA_CRTC_DATA, 0x28);
    outb(VGA_CRTC_INDEX, 0x14);
    outb(VGA_CRTC_DATA, 0x1F);
    outb(VGA_CRTC_INDEX, 0x15);
    outb(VGA_CRTC_DATA, 0x96);
    outb(VGA_CRTC_INDEX, 0x16);
    outb(VGA_CRTC_DATA, 0xB9);
    outb(VGA_CRTC_INDEX, 0x17);
    outb(VGA_CRTC_DATA, 0xA3);

    // Graphics Controller Registers
    outb(VGA_GC_INDEX, 0x00);
    outb(VGA_GC_DATA, 0x00);
    outb(VGA_GC_INDEX, 0x01);
    outb(VGA_GC_DATA, 0x00);
    outb(VGA_GC_INDEX, 0x02);
    outb(VGA_GC_DATA, 0x00);
    outb(VGA_GC_INDEX, 0x03);
    outb(VGA_GC_DATA, 0x00);
    outb(VGA_GC_INDEX, 0x04);
    outb(VGA_GC_DATA, 0x00);
    outb(VGA_GC_INDEX, 0x05);
    outb(VGA_GC_DATA, 0x40);
    outb(VGA_GC_INDEX, 0x06);
    outb(VGA_GC_DATA, 0x05);
    outb(VGA_GC_INDEX, 0x07);
    outb(VGA_GC_DATA, 0x0F);
    outb(VGA_GC_INDEX, 0x08);
    outb(VGA_GC_DATA, 0xFF);

    // Attribute Controller Registers
    for (uint8 i = 0; i < 0x10; ++i)
    {
        inb(VGA_INSTAT_READ);
        outb(VGA_AC_INDEX, i);
        outb(VGA_AC_WRITE, i);
    }
    for (uint8 i = 0x10; i < 0x20; ++i)
    {
        inb(VGA_INSTAT_READ);
        outb(VGA_AC_INDEX, i);
        outb(VGA_AC_WRITE, 0x00);
    }
    inb(VGA_INSTAT_READ);
    outb(VGA_AC_INDEX, 0x20);
}
uint8 *BufferA;
uint8 BufferB[64000];

void clear_screen()
{
    for (int i = 0; i < 64000; i++)
    {
        video_buffer[i] = 0;
    }
}

void swap_buffers()
{
    for (int i = 0; i < 64000; i++)
    {
        BufferA[i] = BufferB[i];
    }
}

void draw()
{
    for (int i = 0; i < 64000; i++)
    {
        video_buffer[i] = BufferA[i];
    }
}
int frame_no = 0;

int timer_tick = 0;

int get_timer_tick()
{
    return timer_tick;
}

void vga_update()
{
    clear_screen();
    draw();
    ++timer_tick;
}

void put_pixel(int x, int y, uint8 color)
{
    BufferB[x + (y * 320)] = color;
}

void render_character(char c, int x_off, int y_off, char col)
{
    char *bitmap = get_font_bitmap(c);
    int x, y;
    int set;
    int mask;
    for (x = 0; x < 8; x++)
    {
        for (y = 0; y < 8; y++)
        {
            set = bitmap[y] & 1 << x;
            if (set)
            {
                put_pixel(x + x_off, y + y_off, col);
            }
        }
    }
}

void setup_video_mode()
{
    set_vga_mode_13h();
    clear_screen();
    init_timer(60, vga_update);
    BufferA = video_buffer;
    for (int i = 0; i < 64000; ++i)
    {
        BufferB[i] = 0;
    }
}