/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * snake.h - Structures and functions for storing and manipulating 
 * the data that represents the snake in the game.
 */

#ifndef SNAKE_H
#define SNAKE_H

#include <SDL3/SDL.h>

#define SNAKE_MAX_SIZE 16384

/* Direction enum for the snake's direction */
enum direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct snake_node
{
    SDL_FRect rect;
    struct snake_node *next;
    struct snake_node *prev;
};

struct snake
{
    struct snake_node *head;
    struct snake_node *tail;
    int size;
    enum direction direction;
};

/* Initialize the snake with values 0 and default direction UP */
extern struct snake *init_snake();

/* Push an SDL_FRect to the front of the snake deque */
extern void push_front(struct snake *snake, SDL_FRect rect);

/* Remove the backmost element from the snake dequeue */
extern void pop_back(struct snake *snake);

/* Get a segment from the snake deque relative to the head */
extern SDL_FRect *get_segment(struct snake *snake, int index);

/* Free snake */
extern void free_snake(struct snake *snake);

#endif
