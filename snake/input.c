/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: MIT
 */

#include <SDL3/SDL.h>
#include "input.h"
#include "snake.h"

void
get_user_input(struct snake *snake, const struct keybinds *keybinds, bool *running)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
			*running = false;

		if (event.type == SDL_EVENT_KEY_DOWN)
		{
			SDL_Keycode key = event.key.key;

			if (key == keybinds->up && snake->direction != DOWN)
				snake->direction = UP;
			else if (key == keybinds->down && snake->direction != UP)
				snake->direction = DOWN;
			else if (key == keybinds->left && snake->direction != RIGHT)
				snake->direction = LEFT;
			else if (key == keybinds->right && snake->direction != LEFT)
				snake->direction = RIGHT;
		}
	}
}

/**
 * @brief Get the direction which would create the longest path for the snake
 *
 * @param snake The snake to find the direction for 
 * @param cell_size The size of a grid cell on the board 
 * @param window_width The width of the playable window 
 * @param window_height The height of the playable window
 * @return enum direction The direction the snake should go to go on the longest possible path
 */
static enum direction
get_longest_direction(const struct snake *snake, int cell_size, int window_width, int window_height)
{
	int ix = (int)(snake->head->rect.x / cell_size);
	int iy = (int)(snake->head->rect.y / cell_size);
	int grid_w = (int)(window_width / cell_size);
	int grid_h = (int)(window_height / cell_size);

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
get_cpu_input(struct snake *snake, const SDL_FRect *apple, bool *running, int cell_size, int window_width, int window_height)
{
	/* Check if user quit */
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
			*running = false;
	}

	SDL_FRect *current_head = snake_get_segment(snake, 0);
	snake->direction = get_longest_direction(snake, cell_size, window_width, window_height);
}
