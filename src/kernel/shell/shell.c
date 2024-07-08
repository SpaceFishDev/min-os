#include "shell.h"
#include "../utils/util.h"
#include "../video/video.h"
#include "../keyboard/keyboard.h"
#include "../heap/heap.h"
#include "../std/stdio.h"
char *screen_buffer;

#define TXT_W ((VGA_W) / 9)
#define TXT_H (((VGA_H) / 9) - 1)
int terminal_col = 15;
bool print_stdin = true;

int cursor_x, cursor_y;
char *command_buffer;

char *stdout;
char *stdin;
char *stderr;

void init_shell()
{
    command_buffer = malloc(TXT_W);
    cursor_x = 2; // resets to 2 to allow for '> '
    cursor_y = 0;
    screen_buffer = malloc(TXT_H * TXT_W * 8);
    for (int i = 0; i < TXT_H * TXT_W; ++i)
    {
        screen_buffer[i] = 0;
    }
    draw_char('>', 0, 0);
    draw_char(' ', 1, 0);
    stdout = malloc(8 * 1024);
    stdin = malloc(8 * 1024);
    stderr = malloc(8 * 1024);
    for (int i = 0; i < (8 * 1024); ++i)
    {
        stdout[i] = 0;
        stdin[i] = 0;
        stderr[i] = 0;
    }
}

void draw_char(char c, int x, int y)
{
    screen_buffer[x + (y * TXT_W)] = c;
}

typedef struct
{
    char *cmd;
    char **arguments;
} shell_command;

shell_command parse_command(char *cmd)
{
    int cnt = 0;
    for (; cmd[cnt] || cmd[cnt] != ' '; ++cnt)
        ;
    char *command = malloc(cnt + 1);
    for (int i = 0; cmd[i] || cmd[i] != ' '; ++i)
    {
        command[i] = cmd[i];
    }
    shell_command out;
    out.cmd = command;

    cmd += cnt + 1;

    if (!cmd)
    {
        return out;
    }

    int space_count = 1;
    for (int i = 0; cmd[i]; ++i)
    {
        if (cmd[i] == ' ')
        {
            space_count++;
        }
    }
    char *list = malloc(space_count * sizeof(char *));
    int count = 0;
    int ls_idx = 0;
    for (int i = 0; cmd[i]; i++)
    {
        if (cmd[i] == ' ')
        {
            int x = i - count;
            char *str = malloc(count);
            for (int z = 0; z < count; ++z)
            {
                str[z] = cmd[x];
                ++x;
            }
            list[ls_idx] = str;
            ++ls_idx;
        }
        ++count;
    }
    out.arguments = list;
    // Remember to free out.arguments and out.arguments[n]
    return out;
}

void shell_putc(char c)
{
    if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
        return;
    }
    screen_buffer[cursor_x + (cursor_y * TXT_W)] = c;
    ++cursor_x;
    if (cursor_x > TXT_W)
    {
        cursor_x = 0;
        cursor_y++;
    }
}
int last_stdout_idx = 0;

int shell_scroll = 0;
bool endl = false;

void set_endl(bool x)
{
    endl = x;
}

void stdout_flush()
{
    bool printed = last_stdout_idx != 0 && (last_stdout_idx != stdout_index);
    for (int i = 0; i < stdout_index; ++i)
    {
        shell_putc(stdout[i]);
    }
    last_stdout_idx = stdout_index;
    stdout_index = 0;
    if (printed && endl)
    {
        cursor_y++;
        cursor_x = 0;
        shell_putc('>');
        shell_putc(' ');
    }
}

void swap_lines(int y1, int y2)
{
    char buf[TXT_W];
    for (int x = 0; x < TXT_W; ++x)
    {
        buf[x] = screen_buffer[x + (y1 * TXT_W)];
    }
    for (int x = 0; x < TXT_W; ++x)
    {
        screen_buffer[x + (y1 * TXT_W)] = screen_buffer[x + (y2 * TXT_W)];
        screen_buffer[x + (y2 * TXT_W)] = buf[x];
    }
}

void update_shell()
{
    char c = poll_keyboard();
    if (c)
    {
        stdin[stdin_index] = c;
        ++stdin_index;
        if (print_stdin)
        {
            putc(c);
        }
    }
    stdout_flush();
    if (cursor_y > TXT_H)
    {
        cursor_y -= 2;
    }
}

void render_shell()
{
    for (int x = 0; x < TXT_W; ++x)
    {
        for (int y = 0; y < TXT_H; ++y)
        {
            int vga_x = x * 9;
            int vga_y = (y * 9) + 9;
            if (vga_y > 0)
            {
                render_character(screen_buffer[x + (y * TXT_W)], vga_x, vga_y, terminal_col);
            }
        }
    }
}