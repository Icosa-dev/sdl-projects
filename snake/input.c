/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "definitions.h"
#include "input.h"
#include "snake.h"

void get_user_input(struct snake *snake, bool *running)
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

/* Check if a snake segment is at a certain coordinate */
static bool segment_at(struct snake *snake, float x, float y)
{
	for (int i = 1; i < snake->size; i++)
	{
		if (get_segment(snake, i)->x == x && get_segment(snake, i)->y == y)
			return true;
	}

	return false;
}

/* Check if a move is valid and will not cause collision */
static bool is_valid(struct snake *snake, float upper_bound_x,
	float upper_bound_y, float lower_bound_x, float lower_bound_y,
	enum direction move_direction)
{
	float current_x = get_segment(snake, 0)->x;
	float current_y = get_segment(snake, 0)->y;
	float next_x = current_x;
	float next_y = current_y;

	switch (move_direction)
	{
	case UP:
		next_y -= CELL_SIZE;
		break;
	case DOWN:
		next_y += CELL_SIZE;
		break;
	case LEFT:
		next_x -= CELL_SIZE;
		break;
	case RIGHT:
		next_x += CELL_SIZE;
		break;
	}

	/* Check if the new coordinates hit the body */
	for (int i = 1; i < snake->size; i++)
	{
		if (next_x == get_segment(snake, i)->x &&
			next_y == get_segment(snake, i)->y)
			return false;
	}

	/* Check if the new coordinates hit the board boundaries */
	if (next_x <= lower_bound_x || next_y <= lower_bound_y ||
		next_x >= upper_bound_x || next_y >= upper_bound_y)
		return false;

	if (segment_at(snake, next_x, next_y))
		return false;

	return true;
}

static enum direction get_longest_direction(float x, float y,
	float upper_bound_x, float upper_bound_y)
{
	int ix = (int)(x / CELL_SIZE);
	int iy = (int)(y / CELL_SIZE);
	int grid_w = (int)(upper_bound_x / CELL_SIZE);
	int grid_h = (int)(upper_bound_y / CELL_SIZE);

	if (ix == 0 && iy > 0)
	{
		return UP;
	}

	if (iy == 0 && ix < grid_w - 1)
	{
		return RIGHT;
	}

	if (ix == grid_w - 1 && iy % 2 == 0)
	{
		return DOWN;
	}
	if (ix == 1 && iy % 2 != 0 && iy < grid_h - 1)
	{
		return DOWN;
	}

	if (iy == grid_h - 1 && ix > 0)
	{
		return LEFT;
	}

	if (iy % 2 == 0)
	{
		return RIGHT;
	} else
	{
		return LEFT;
	}
}

/*
 * This function will set snake to a different direction than
 * provided if the given one is not valid
 */
static void move_snake(struct snake *snake, enum direction direction,
	float upper_bound_x, float upper_bound_y, float lower_bound_x,
	float lower_bound_y)
{
#define IS_VALID(direction)                                      \
	is_valid(snake, upper_bound_x, upper_bound_y, lower_bound_x, \
		lower_bound_y, direction)

	if (IS_VALID(direction))
		snake->direction = direction;

	else if (IS_VALID(UP))
		snake->direction = UP;
	else if (IS_VALID(DOWN))
		snake->direction = DOWN;
	else if (IS_VALID(LEFT))
		snake->direction = LEFT;
	else if (IS_VALID(RIGHT))
		snake->direction = RIGHT;

#undef IS_VALID
}

/*
 * Finally! The first snake game to be fully AI powered!
 * Truely the future of software!
 */
void get_cpu_input(struct snake *snake, const SDL_FRect *apple,
	float upper_bound_x, float upper_bound_y, float lower_bound_x,
	float lower_bound_y, bool *running)
{
	/* Check if user quit */
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
			*running = false;
	}

	SDL_FRect *current_head = get_segment(snake, 0);
	snake->direction = get_longest_direction(current_head->x,
		current_head->y, upper_bound_x, upper_bound_y);
}
