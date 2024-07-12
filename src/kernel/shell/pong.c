#include "pong.h"

#include "../keyboard/keyboard.h"
#include "../std/stdio.h"
#include "../heap/heap.h"
float paddle_1_x = 10;
float paddle_1_y = 90;
float paddle_2_x = 310;
float paddle_2_y = 90;

float ball_x = 160;
float ball_y = 100;

float vx = 0.02;
float vy = 0;

void draw_paddle()
{
    for (int i = 0; i < 15; i++)
    {
        put_pixel(paddle_1_x, paddle_1_y + i, 15);
        put_pixel(paddle_2_x, paddle_2_y + i, 15);
    }
}

int score_1 = 0;
int score_2 = 0;

void pong(int argc, char **argv)
{
    bool quit = false;

    bool up_1 = false;
    bool up_2 = false;
    bool down_1 = false;
    bool down_2 = false;

    char *score_1_str = itoa(score_1);
    char *score_2_str = itoa(score_2);
    while (!quit)
    {
        clear_back_buffer();

        render_string(score_1_str, 140, 10, 15);
        render_string(score_2_str, 160, 10, 15);
        render_string("Press q or escape to quit.", 0, 190, 15);

        char c = poll_keyboard();
        if (c == ESC || c == 'q')
        {
            quit = true;
        }
        if (c == 'w')
        {
            paddle_1_y -= 2;
            up_1 = true;
            down_1 = false;
        }
        if (c == 'i')
        {
            paddle_2_y -= 2;
            up_2 = true;
            down_2 = false;
        }
        if (c == 'k')
        {
            paddle_2_y += 2;
            down_2 = true;
            up_2 = false;
        }
        if (c == 's')
        {
            paddle_1_y += 2;
            down_1 = true;
            up_1 = false;
        }
        draw_paddle();
        put_pixel(ball_x, ball_y, 15);
        ball_x += vx;
        ball_y += vy;
        if (ball_x >= 320)
        {
            score_1++;
            ball_x = 160;
            ball_y = 100;
            vy = 0;
            score_1_str = itoa(score_1);
        }
        if (ball_x <= 0)
        {
            score_2++;
            ball_x = 160;
            ball_y = 100;
            vy = 0;
            score_2_str = itoa(score_2);
        }
        if (ball_y <= 20 || ball_y >= 180)
        {
            vy = -vy;
        }
        if (ball_x >= 310)
        {
            if (ball_y >= paddle_2_y && ball_y <= paddle_2_y + 15)
            {
                vx = -vx;
                if (up_2)
                {
                    vy = -0.005;
                    up_2 = false;
                }
                if (down_2)
                {
                    vy = 0.005;
                    down_2 = false;
                }
            }
        }
        if (ball_x <= 10)
        {
            if (ball_y >= paddle_1_y && ball_y <= paddle_1_y + 15)
            {
                vx = -vx;
                if (up_1)
                {
                    vy = -0.005;
                    up_1 = false;
                }
                if (down_1)
                {
                    vy = 0.005;
                    down_1 = false;
                }
            }
        }
        swap_buffers();
    }
    free(score_1_str);
    free(score_2_str);
    ball_x = 160;
    ball_y = 100;
    paddle_1_x = 10;
    paddle_1_y = 90;
    paddle_2_x = 310;
    paddle_2_y = 90;
    score_1 = 0;
    score_2 = 0;
    clear_back_buffer();
    clear_screen();
}