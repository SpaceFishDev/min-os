#include "pong.h"

#include "../keyboard/keyboard.h"
#include "../std/stdio.h"

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

    // bool up_1 = false;
    // bool up_2 = false;
    // bool down_1 = false;
    // bool down_2 = false;
    int sync = get_timer_tick();
    clear_back_buffer();
    while (!quit)
    {
        int time_cur = get_timer_tick();
        if (time_cur > sync)
        {

            //     char *score_1_str = itoa(score_1);

            //     render_string(score_1_str, 50, 10, 15);

            char c = poll_keyboard();
            if (c == ESC || c == 'q')
            {
                quit = true;
            }
            //     if (c == 'w')
            //     {
            //         paddle_1_y -= 2;
            //         up_1 = true;
            //     }
            //     if (c == 'i')
            //     {
            //         paddle_2_y -= 2;
            //         up_2 = true;
            //     }
            //     if (c == 'k')
            //     {
            //         paddle_2_y += 2;
            //         down_1 = true;
            //     }
            //     if (c == 's')
            //     {
            //         paddle_1_y += 2;
            //         down_2 = true;
            //     }
            //     draw_paddle();
            //     put_pixel(ball_x, ball_y, 15);
            //     ball_x += vx;
            //     ball_y += vy;
            //     if (ball_x >= 320)
            //     {
            //         score_1++;
            //         ball_x = 160;
            //         ball_y = 100;
            //     }
            //     if (ball_y <= 0 || ball_y >= 200)
            //     {
            //         vy = -vy;
            //     }
            //     if (ball_x >= 310)
            //     {
            //         if (ball_y >= paddle_2_y && ball_y <= paddle_2_y + 15)
            //         {
            //             vx = -vx;
            //             if (up_2)
            //             {
            //                 vy = -0.005;
            //                 up_2 = false;
            //             }
            //         }
            //     }
            //     if (ball_x <= 10)
            //     {
            //         if (ball_y >= paddle_1_y && ball_y <= paddle_1_y + 15)
            //         {
            //             vx = -vx;
            //             if (up_1)
            //             {
            //                 vy = -0.005;
            //                 up_1 = false;
            //             }
            //         }
            //     }
            swap_buffers();
            sync = time_cur;
            time_cur = get_timer_tick();
        }
    }
}