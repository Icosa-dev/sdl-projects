/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: MIT
 *
 * main.c - Entry point for the game
 *
 * A basic snake game implemented in SDL3. Keybinds are WASD and can be
 * changed by updating the KEYBIND macros. If you want the CPU to play for
 * you pass the argument "snake cpu".
 */

#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "input.h"
#include "snake.h"

/* String comparison macro for argparsing */
#define STREQ(str1, str2) (strcmp(str1, str2) == 0)

/* Generate random possition values for apple generation */
static void get_rand_pos(float *out_x, float *out_y, struct snake *snake,
	const struct config *config)
{
	bool overlapping;
	do
	{
		overlapping = false;

		*out_x = (float)(SDL_rand(config->columns) * config->cell_size);
		*out_y = (float)(SDL_rand(config->rows) * config->cell_size);

		for (int i = 0; i < snake->size; i++)
		{
			SDL_FRect *seg = get_segment(snake, i);

			if (*out_x == seg->x && *out_y == seg->y)
			{
				overlapping = true;
				break;
			}
		}
	} while (overlapping);
}

/* Freeze the screen when game over */
static void game_over_screen(SDL_Renderer *renderer)
{
	SDL_Event event;
	for (;;)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
			{
				SDL_Quit();
				exit(0);
			}
		}
	}
}

int main(int argc, char **argv)
{
	struct config config = STD_CONFIG;

	if (argc > 1)
	{
		if (STREQ(argv[1], "help"))
		{
			printf("Usage: snake <OPTIONS>\n");
			printf("Options:\n");
			printf("\thelp:\t\tPrint help message\n");
			printf("\tcpu:\t\tHave the computer play for you\n");
		} else if (STREQ(argv[1], "cpu"))
		{
			config.cpu_enabled = true;
			config.delay = 1;
		}
	}

	/* Initialize SDL */
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	/* Create window and renderer */
	SDL_Window *window = SDL_CreateWindow(config.title, config.window_width,
		config.window_height, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

	/* Create snake */
	struct snake *snake = init_snake();
	/*
	 * Push the first segment of the snake with
	 * possition in the middle of the window
	 */
	push_front(snake,
		(SDL_FRect) { (float)config.window_width / 2,
			(float)config.window_height / 2, config.cell_size,
			config.cell_size });

	/* Create apple */
	SDL_FRect apple = (SDL_FRect) { 0, 0, config.cell_size, config.cell_size };
	get_rand_pos(&(apple.x), &(apple.y), snake, &config);

	/* Game loop */
	bool running = true;
	while (running)
	{
		/* Event handling */
		if (config.cpu_enabled)
			get_cpu_input(snake, &apple, &config, &running);
		else
			get_user_input(snake, &config, &running);

		SDL_FRect new_head = snake->head->rect;
		switch (snake->direction)
		{
		case UP:
			new_head.y -= config.cell_size;
			break;
		case DOWN:
			new_head.y += config.cell_size;
			break;
		case LEFT:
			new_head.x -= config.cell_size;
			break;
		case RIGHT:
			new_head.x += config.cell_size;
			break;
		}

		/* Snake-apple collision detection */
		bool ate_apple = false;
		if (new_head.x == apple.x && new_head.y == apple.y)
		{
			ate_apple = true;
			get_rand_pos(&(apple.x), &(apple.y), snake, &config);
		}

		/* Snake-border collision detection */
		if (new_head.x == (-config.cell_size) ||
			new_head.x == config.window_width ||
			new_head.y == (-config.cell_size) ||
			new_head.y == config.window_height)
			game_over_screen(renderer);

		/* Update snake body */
		push_front(snake, new_head);
		if (!ate_apple)
			pop_back(snake); /* remove back segment if not growing */

		/* Snake-snake collision detection */
		for (int i = 1; i < snake->size; i++)
		{
			SDL_FRect *segment = get_segment(snake, i);
			if (new_head.x == segment->x && new_head.y == segment->y)
				game_over_screen(renderer);
		}

		/* Clear renderer */
		SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(config.board_color));
		SDL_RenderClear(renderer);

		/* Render snake */
		SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(config.snake_color));
		for (int i = 0; i < snake->size; i++)
		{
			SDL_RenderFillRect(renderer, get_segment(snake, i));
		}

		/* Render apple */
		SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(config.apple_color));
		SDL_RenderFillRect(renderer, &apple);

		/* Present rendered frame */
		SDL_RenderPresent(renderer);
		SDL_Delay(config.delay);
	}

	/* Free snake */
	free_snake(snake);

	/* Free SDL resources */
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
