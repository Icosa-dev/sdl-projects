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
