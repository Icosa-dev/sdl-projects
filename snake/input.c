/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "input.h"
#include "snake.h"

void get_input(struct snake *snake, bool *running)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
            *running = false;

        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            switch (event.key.key)
            {
            case KEYBIND_UP:
                if (snake->direction != DOWN)
                    snake->direction = UP;
                break;
            case KEYBIND_DOWN:
                if (snake->direction != UP)
                    snake->direction = DOWN;
                break;
            case KEYBIND_LEFT:
                if (snake->direction != RIGHT)
                    snake->direction = LEFT;
                break;
            case KEYBIND_RIGHT:
                if (snake->direction != LEFT)
                    snake->direction = RIGHT;
                break;
            }
        }
    }
}

void get_cpu_input(struct snake *snake, bool *running)
{
    static int alternator = 0;

    /* Check if user quit */
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
            *running = false;
    }

    switch (alternator)
    {
    case 0:
        snake->direction = UP; alternator++; break;
    case 1:
        snake->direction = RIGHT; alternator++; break;
    case 2:
        snake->direction = DOWN; alternator++; break;
    case 3:
        snake->direction = RIGHT; alternator = 0; break;
    }
}
