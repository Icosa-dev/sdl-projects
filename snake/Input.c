/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Input.h"
#include "Snake.h"

void GetInput(Snake *snake, bool *running)
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
