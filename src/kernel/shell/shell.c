#include "shell.h"
#include "../utils/util.h"
#include "../video/video.h"
#include "../keyboard/keyboard.h"
#include "../heap/heap.h"
#include "../std/stdio.h"
char *screen_buffer;

#define TXT_W ((VGA_W) / 9)
#define TXT_H (((VGA_H) / 10) - 1)
int terminal_col = 15;
bool print_stdin = true;

int cursor_x, cursor_y;
char *command_buffer;

char *stdout;
char *stdin;
char *stderr;

typedef void (*command_handler_t)(int argc, char **argv);
command_handler_t *command_handlers;

enum command_table
{
    CMD_PONG,
    NUM_CMD,
};

extern command_handler_t pong(int argc, char **argv);

void init_shell()
{

    command_handlers = malloc(sizeof(command_handlers) * NUM_CMD);
    command_handlers[CMD_PONG] = pong;
    command_buffer = malloc(TXT_W);
    screen_buffer = malloc(TXT_H * TXT_W * 8);
    for (int i = 0; i < TXT_H * TXT_W; ++i)
    {
        screen_buffer[i] = 0;
    }
    shell_putc('>');
    shell_putc(' ');
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
    int arg_cnt;
    char **arguments;
} shell_command;

void free_shellcmd(shell_command cmd)
{
    free(cmd.cmd);
    for (int i = 0; i < cmd.arg_cnt; ++i)
    {
        free(cmd.arguments[i]);
    }
    free(cmd.arguments);
}

shell_command parse_command(char *cmd)
{
    int cnt = 0;
    for (; cmd[cnt]; ++cnt)
    {
        if (cmd[cnt] == '\n')
        {
            break;
        }
    }
    char *str = malloc(cnt + 1);
    bool space_found = false;
    int i = 0;
    for (i = 0; i < cnt; ++i)
    {
        if (cmd[i] == ' ')
        {
            space_found = true;
            break;
        }
        str[i] = cmd[i];
    }
    str[cnt] = 0;
    if (!space_found)
    {
        shell_command scmd;
        scmd.cmd = str;
        scmd.arg_cnt = 0;
        return scmd;
    }
    int num_arg = 0;
    int x = i;
    for (; i < cnt; ++i)
    {
        if (cmd[i] == ' ')
        {
            ++num_arg;
        }
    }

    char **ls = (char **)malloc(num_arg * sizeof(char **));
    int n_ls = 0;
    ++x;
    int buf_idx = 0;

    for (; x < cnt; ++x)
    {
        if (cmd[x] == ' ')
        {

            char *str = malloc(buf_idx + 1);
            str[buf_idx] = 0;
            int n = 0;

            for (int z = x - buf_idx; n < buf_idx; ++z)
            {
                str[n] = cmd[z];
                ++n;
            }
            ls[n_ls] = str;
            ++n_ls;
            buf_idx = 0;
        }
        else
        {
            ++buf_idx;
        }
    }
    char *str2 = malloc(buf_idx + 1);
    str2[buf_idx] = 0;
    int n = 0;

    for (int z = x - buf_idx; n < buf_idx; ++z)
    {
        str2[n] = cmd[z];
        ++n;
    }
    ls[n_ls] = str2;
    shell_command out;
    out.cmd = str;
    out.arguments = ls;
    out.arg_cnt = num_arg;
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
bool shell_enable = true;

void disable_shell()
{
    shell_enable = false;
}

void enable_shell()
{
    shell_enable = true;
}

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
        putc('>');
        putc(' ');
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

char cmd_buf[1024];
int cmd_idx = 0;

void handle_command(char c)
{
    char *cmd_str;
    int len = 0;
    int cnt = 0;
    for (int i = 0; i < cmd_idx; ++i)
    {
        if (cmd_buf[i] == '\n')
        {
            ++cnt;
        }
        if (cnt == 1)
        {
            break;
        }
        ++len;
    }
    cmd_str = malloc(len + 1);
    for (int i = 0; i < len; ++i)
    {
        cmd_str[i] = cmd_buf[i];
    }
    cmd_str[len] = 0;
    shell_command cmd = parse_command(cmd_str);
    if (strcmp(cmd.cmd, "pong"))
    {
        pong(cmd.arg_cnt, cmd.arguments);
    }
    if (strcmp(cmd.cmd, "clear"))
    {
        for (int i = 0; i < TXT_W * TXT_H; ++i)
        {
            screen_buffer[i] = 0;
        }

        putc('>');
        putc(' ');
        cursor_x = 0;
        cursor_y = 0;
        clear_back_buffer();
        free(cmd_str);
        free_shellcmd(cmd);
        int x = cmd_idx;
        cmd_idx = 0;
        for (; x > 0; --x)
        {
            cmd_buf[x] = 0;
        }
        return;
    }
    free(cmd_str);
    free_shellcmd(cmd);
    int x = cmd_idx;
    cmd_idx = 0;
    for (; x > 0; --x)
    {
        cmd_buf[x] = 0;
    }
    putc('\n');
    putc('>');
    putc(' ');
}

void update_shell()
{
    if (!shell_enable)
    {
        return;
    }
    char c = poll_keyboard();
    if (c == '\n')
    {
        handle_command(c);
    }
    else if (c == BS)
    {
        int j = cmd_idx - 1;
        for (int i = cursor_x - 1; i < TXT_W; ++i)
        {
            screen_buffer[i + (cursor_y * TXT_W)] = screen_buffer[i + 1 + (cursor_y * TXT_W)];

            cmd_buf[j] = cmd_buf[j + 1];

            if (screen_buffer[i + 1] == 0)
            {
                break;
            }
            ++cmd_idx;
        }
        --cursor_x;
        --cmd_idx;
    }
    else if (c)
    {
        stdin[stdin_index] = c;
        cmd_buf[cmd_idx++] = c;
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
    if (!shell_enable)
    {
        return;
    }
    clear_back_buffer();
    for (int x = 0; x < TXT_W; ++x)
    {
        for (int y = 0; y < TXT_H; ++y)
        {
            int vga_x = x * 9;
            int vga_y = (y * 10) + 10;
            if (vga_y > 0)
            {
                render_character(screen_buffer[x + (y * TXT_W)], vga_x, vga_y, terminal_col);
            }
        }
    }
}