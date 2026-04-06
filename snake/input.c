/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: MIT
 */

#include "config.h"
#include "input.h"
#include "snake.h"

void
get_user_input(struct snake *snake, const struct config *config, bool *running)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
			*running = false;

		if (event.type == SDL_EVENT_KEY_DOWN)
		{
			SDL_Keycode key = event.key.key;

			if (key == config->keybind_up && snake->direction != DOWN)
				snake->direction = UP;
			else if (key == config->keybind_down && snake->direction != UP)
				snake->direction = DOWN;
			else if (key == config->keybind_left && snake->direction != RIGHT)
				snake->direction = LEFT;
			else if (key == config->keybind_right && snake->direction != LEFT)
				snake->direction = RIGHT;
		}
	}
}

static enum direction
get_longest_direction(float x, float y, const struct config *config)
{
	int ix = (int)(x / config->cell_size);
	int iy = (int)(y / config->cell_size);
	int grid_w = (int)(config->window_width / config->cell_size);
	int grid_h = (int)(config->window_height / config->cell_size);

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
void
get_cpu_input(struct snake *snake, const SDL_FRect *apple,
	const struct config *config, bool *running)
{
	/* Check if user quit */
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
			*running = false;
	}

	SDL_FRect *current_head = get_segment(snake, 0);
	snake->direction = get_longest_direction(current_head->x, current_head->y,
		config);
}
