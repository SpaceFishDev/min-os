#ifndef VIDEO_H

#define VIDEO_H
#include "../utils/util.h"
void setup_video_mode();
int get_timer_tick();
void put_pixel(int x, int y, uint8 color);
void render_character(char c, int x_off, int y_off, char col);
void swap_buffers();

#endif