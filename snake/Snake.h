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
#include "Definitions.h"

#define SNAKE_MAX_SIZE (ROWS * COLUMNS)

// Direction enum for the snake's direction
typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

// Snake is a static double-ended queue of SDL_FRects
typedef struct
{
    SDL_FRect elements[SNAKE_MAX_SIZE];
    int head;
    int size;
    Direction direction;
} Snake;

// Initialize the snake with values 0 and default direction UP
extern void InitSnake(Snake *snake);

// Push an SDL_FRect to the front of the snake deque
extern void PushBack(Snake *snake, SDL_FRect rect);

// Remove the backmost element from the snake dequeue
extern void PopBack(Snake *snake);

// Get a segment from the snake deque relative to the head
extern SDL_FRect *GetSegment(Snake *snake, int index);

#endif
