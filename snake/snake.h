/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef SNAKE_H_
#define SNAKE_H_

#include <SDL3/SDL_rect.h>

enum direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

struct snake_node
{
	SDL_FRect	   rect;
	struct snake_node *prev;
	struct snake_node *next;
};

struct snake
{
	struct snake_node *head;
	struct snake_node *tail;
	size_t		   size;
	enum direction	   direction;
};

/**
 * @brief Allocate and initialize a snake structure
 *
 * @return struct snake* Pointer to the new snake structure
 */
extern struct snake *snake_init();

/**
 * @brief Push an SDL_FRect to the front of the snake body
 *
 * @param snake The snake to be modified
 * @param rect The SDL_FRect to push to the body
 */
extern void snake_push_front(struct snake *snake, SDL_FRect rect);

/**
 * @brief Remove the backmost element from the snake body
 *
 * @param snake The snake to be modified
 */
extern void snake_pop_back(struct snake *snake);

/**
 * @brief Get a pointer to a segment of the snake body relative to the head
 *
 * @param snake The snake to get the segment from
 * @param index The index of the segment relative to the head
 * @return SDL_FRect* Pointer to the segment of the snake body
 */
extern SDL_FRect *snake_get_segment(const struct snake *snake, size_t index);

/**
 * @brief Free all snake nodes and set values of the snake members to NULL or 0
 *
 * @param snake The snake to free from memory
 */
extern void snake_free(struct snake *snake);

#endif
