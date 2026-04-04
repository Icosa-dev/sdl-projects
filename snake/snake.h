/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Snake.h - Structures and functions for storing and manipulating 
 * the data that represents the snake in the game.
 */

#ifndef SNAKE_H
#define SNAKE_H

#include <SDL3/SDL.h>
#include "definitions.h"

#define SNAKE_MAX_SIZE (ROWS * COLUMNS)

/* Direction enum for the snake's direction */
enum direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

/* Snake is a static double-ended queue of SDL_FRects */
struct snake
{
    SDL_FRect elements[SNAKE_MAX_SIZE];
    int head;
    int size;
    enum direction direction;
};

/* Initialize the snake with values 0 and default direction UP */
extern void init_snake(struct snake *snake);

/* Push an SDL_FRect to the front of the snake deque */
extern void push_back(struct snake *snake, SDL_FRect rect);

/* Remove the backmost element from the snake dequeue */
extern void pop_back(struct snake *snake);

/* Get a segment from the snake deque relative to the head */
extern SDL_FRect *get_segment(struct snake *snake, int index);

#endif
