/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef SNAKE_H
#define SNAKE_H

#include <SDL3/SDL_rect.h>

typedef enum { UP, DOWN, LEFT, RIGHT } Direction;

struct _SnakeNode {
    SDL_FRect rect;
    struct _SnakeNode *prev, *next;
};
typedef struct _SnakeNode SnakeNode;

typedef struct {
    SnakeNode *head, *tail;
    int size;
    Direction direction;
} Snake;

/**
 * @brief Allocate and initialize a snake structure
 *
 * @return struct snake* Pointer to the new snake structure
 */
extern Snake *snakeInit(void);

/**
 * @brief Push an SDL_FRect to the front of the snake body
 *
 * @param snake The snake to be modified
 * @param rect The SDL_FRect to push to the body
 */
extern void snakePushFront(Snake *snake, SDL_FRect rect);

/**
 * @brief Remove the backmost element from the snake body
 *
 * @param snake The snake to be modified
 */
extern void snakePopBack(Snake *snake);

/**
 * @brief Get a pointer to a segment of the snake body relative to the head
 *
 * @param snake The snake to get the segment from
 * @param index The index of the segment relative to the head
 * @return SDL_FRect* Pointer to the segment of the snake body
 */
extern SDL_FRect *snakeGetSegment(const Snake *snake, int index);

/**
 * @brief Free all snake nodes and set values of the snake members to NULL or 0
 *
 * @param snake The snake to free from memory
 */
extern void snakeFree(Snake *snake);

#endif
