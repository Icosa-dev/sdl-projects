/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <SDL3/SDL.h>
#include "snake.h"

/* Initialize the snake with values 0 and default direction UP */
void init_snake(struct snake *snake)
{
    snake->head = 0;
    snake->size = 0;
    snake->direction = UP;
}

/* Push an SDL_FRect to the front of the snake deque */
void push_back(struct snake *snake, SDL_FRect rect)
{
    if (snake->size >= SNAKE_MAX_SIZE)
        return;

    snake->head = (snake->head - 1 + SNAKE_MAX_SIZE) % SNAKE_MAX_SIZE;
    snake->elements[snake->head] = rect;
    if (snake->size < SNAKE_MAX_SIZE)
        snake->size++;
}

void pop_back(struct snake *snake)
{
    if (snake->size > 0)
        snake->size--;
}

SDL_FRect *get_segment(struct snake *snake, int index)
{
    int actualIndex = (snake->head + index) % SNAKE_MAX_SIZE;
    return &snake->elements[actualIndex];
}
