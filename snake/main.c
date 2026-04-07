/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: MIT
 */

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <color.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "snake.h"

#define PROGRAM_NAME "snake"
#define DESCRIPTION "A basic snake game implementation in SDL3"
#define AUTHORS "LJC"
#define VERSION "v0.1.0"
#define MIT_LICENSE_TEXT                                                                \
	"Copyright (c) " __DATE__  " " AUTHORS "\n\n"                                       \
	"Permission is hereby granted, free of charge, to any person obtaining a copy\n"    \
	"of this software and associated documentation files (the \"Software\"), to deal\n" \
	"in the Software without restriction, including without limitation the rights\n"    \
	"to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"       \
	"copies of the Software, and to permit persons to whom the Software is\n"           \
	"furnished to do so, subject to the following conditions:\n\n"                      \
	"The above copyright notice and this permission notice shall be included in all\n"  \
	"copies or substantial portions of the Software.\n\n"                               \
	"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"    \
	"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"        \
	"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n"     \
	"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"          \
	"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"   \
	"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"   \
	"SOFTWARE."

#define STREQ(str1, str2) (strcmp(str1, str2) == 0)

static void
print_version(void)
{
	printf("%s: %s (%s)\n%s\n", PROGRAM_NAME, DESCRIPTION, VERSION,
		MIT_LICENSE_TEXT);
	exit(0);
}

static void
print_help(void)
{
	printf("Usage: snake [OPTIONS]\n");
	printf("Options:\n");
	printf("\t-h, --help\t\t\t\t\tPrint this message and exit\n");
	printf("\t-v, --version\t\t\t\t\tPrint version message and exit\n");
	printf("\t--cpu-enabled\t\t\t\t\tEnable CPU to play the game\n");
	printf(
		"\t--cell-size <int>\t\t\t\tSet the number of pixels per board cell\n");
	printf("\t--rows <int>\t\t\t\t\tSet the total number of board rows\n");
	printf(
		"\t--columns <int>\t\t\t\t\tSet the total number of board columns\n");
	printf(
		"\t--keybinds <wasd, arrows>\t\t\tSet which keybinds should be used\n");
	printf(
		"\t--delay <int>\t\t\t\t\tSet the delay of the program in milliseconds\n");
	exit(0);
}

static void
freeze_screen(void)
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

/**
 * @brief Set the x and y value of the rect to a random x and y not inside of
 * the snake body
 *
 * @param rect Input rect
 * @param snake The snake to avoid
 * @param max_x The max x value
 * @param max_y The max y value
 */
static void
get_rand_pos(SDL_FRect *rect, struct snake *snake, int cell_size, int rows,
	int columns)
{
	bool overlapping;
	do
	{
		overlapping = false;

		rect->x = (float)(SDL_rand(columns) * cell_size);
		rect->y = (float)(SDL_rand(rows) * cell_size);

		for (int i = 0; i < snake->size; i++)
		{
			SDL_FRect *seg = snake_get_segment(snake, i);

			if (rect->x == seg->x && rect->y == seg->y)
			{
				overlapping = true;
				break;
			}
		}
	} while (overlapping);
}

int
main(int argc, char **argv)
{
	/* Default game values */
	int cell_size = 25;
	int rows = 20;
	int columns = 20;
	int snake_max_size = rows * columns;

	int window_width = columns * cell_size;
	int window_height = rows * cell_size;

	struct keybinds keybinds = WASD;

	bool cpu_enabled = false;
	int delay = 50;

	/* Argument parsing */
	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			const char *arg = argv[i];

			/* Flag parsing */
			if (STREQ(arg, "-v") || STREQ(arg, "--version"))
				print_version();
			else if (STREQ(arg, "-h") || STREQ(arg, "--help"))
				print_help();
			else if (STREQ(arg, "--cpu-enabled"))
				cpu_enabled = true;

			/* Optional arg parsing */
			if (i + 1 < argc)
			{
				const char *value = argv[i + 1];

				if (STREQ(arg, "--cell-size"))
					cell_size = atoi(value);
				else if (STREQ(arg, "--rows"))
					rows = atoi(value);
				else if (STREQ(arg, "--columns"))
					columns = atoi(value);
				else if (STREQ(arg, "--keybinds"))
				{
					if (STREQ(value, "arrows"))
					{
						keybinds.up = SDLK_UP;
						keybinds.down = SDLK_DOWN;
						keybinds.left = SDLK_LEFT;
						keybinds.right = SDLK_RIGHT;
					}
				} else if (STREQ(arg, "--delay"))
					delay = atoi(value);
				else if (STREQ(arg, "--"))
					break;
			}
		}
	}

	/* Game logic */
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	SDL_Window *window = SDL_CreateWindow(PROGRAM_NAME, window_width,
		window_height, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

	struct snake *snake = snake_init();
	float center_x = (float)window_width / 2;
	float center_y = (float)window_height / 2;
	snake_push_front(snake,
		(SDL_FRect) { center_x, center_y, cell_size, cell_size });

	SDL_FRect apple = (SDL_FRect) { 0, 0, cell_size, cell_size };
	get_rand_pos(&apple, snake, cell_size, rows, columns);

	bool running = true;
	SDL_Event event;
	while (running)
	{
		/* Input handling */
		if (cpu_enabled)
			get_cpu_input(snake, &apple, &running, cell_size, window_height,
				window_width);
		else
			get_user_input(snake, &keybinds, &running);

		/* Move snake */
		SDL_FRect new_head = snake->head->rect;
		switch (snake->direction)
		{
		case UP:
			new_head.y -= cell_size;
			break;
		case DOWN:
			new_head.y += cell_size;
			break;
		case LEFT:
			new_head.x -= cell_size;
			break;
		case RIGHT:
			new_head.x += cell_size;
			break;
		}

		/* Snake-apple collision detection */
		bool ate_apple = false;
		if (new_head.x == apple.x && new_head.y == apple.y)
		{
			ate_apple = true;
			get_rand_pos(&apple, snake, cell_size, rows, columns);
		}

		/* Snake-border collision detection */
		if (new_head.x < 0 || new_head.x == window_width || new_head.y < 0 ||
			new_head.y == window_height)
			freeze_screen();

		/* Update snake body */
		snake_push_front(snake, new_head);
		if (!ate_apple)
			snake_pop_back(snake); /* remove back segment if not growing */

		/* Snake-snake collision detection */
		for (int i = 1; i < snake->size; i++)
		{
			SDL_FRect *segment = snake_get_segment(snake, i);
			if (new_head.x == segment->x && new_head.y == segment->y)
				freeze_screen();
		}

		if (snake->size == snake_max_size)
			freeze_screen();

		/* Clear screen */
		SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(BLACK));
		SDL_RenderClear(renderer);

		/* Render apple */
		SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(RED));
		SDL_RenderFillRect(renderer, &apple);

		/* Render snake */
		SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(WHITE));
		for (int i = 0; i < snake->size; i++)
		{
			SDL_RenderFillRect(renderer, snake_get_segment(snake, i));
		}

		/* Present renderer */
		SDL_RenderPresent(renderer);

		SDL_Delay(delay);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
