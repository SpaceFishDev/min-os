#ifndef SHELL_H
#define SHELL_H
#include "../utils/debug/debug.h"
#include "../video/video.h"

void init_shell();
void draw_char(char c, int x, int y);
void render_shell();
void update_shell();
void disable_shell();
void enable_shell();

#endif