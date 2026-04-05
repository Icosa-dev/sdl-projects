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

#include "definitions.h"
#include "input.h"
#include "snake.h"

/* Window values */
#define TITLE "Snake"
#define WINDOW_WIDTH (COLUMNS * CELL_SIZE)
#define WINDOW_HEIGHT (ROWS * CELL_SIZE)
#define UPPER_BOUND_X WINDOW_WIDTH
#define UPPER_BOUND_Y WINDOW_HEIGHT
#define LOWER_BOUND_X (-CELL_SIZE)
#define LOWER_BOUND_Y (-CELL_SIZE)

/* String comparison macro for argparsing */
#define STREQ(str1, str2) (strcmp(str1, str2) == 0)

/* Generate random possition values for apple generation */
static void get_rand_pos(float *out_x, float *out_y, struct snake *snake)
{
	bool overlapping;
	do
	{
		overlapping = false;

		*out_x = (float)(SDL_rand(COLUMNS) * CELL_SIZE);
		*out_y = (float)(SDL_rand(ROWS) * CELL_SIZE);

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
	for (;;) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				SDL_Quit();
				exit(0);
			}
		}
	}
}

int main(int argc, char **argv)
{
	bool CPUEnabled = false;
	int delay = 50;

	if (argc > 1) {
		if (STREQ(argv[1], "help")) {
			printf("Usage: snake <OPTIONS>\n");
			printf("Options:\n");
			printf("\thelp:\t\tPrint help message\n");
			printf("\tcpu:\t\tHave the computer play for you\n");
		} else if (STREQ(argv[1], "cpu")) {
			CPUEnabled = true; delay = 1;
		}
	}

	/* Initialize SDL */
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	/* Create window and renderer */
	SDL_Window *window =
		SDL_CreateWindow(TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

	/* Create snake */
	struct snake snake;
	init_snake(&snake);
	/* 
	 * Push the first segment of the snake with
     * possition in the middle of the window
     */
	push_back(&snake, (SDL_FRect){ (float)WINDOW_WIDTH / 2,
				       (float)WINDOW_HEIGHT / 2, CELL_SIZE,
				       CELL_SIZE });

	/* Create apple */
	SDL_FRect apple =
		(SDL_FRect){ 0, 0, CELL_SIZE, CELL_SIZE };
	get_rand_pos(&(apple.x), &(apple.y), &snake);

	/* Game loop */
	bool running = true;
	while (running) {
		/* Event handling */
		if (CPUEnabled)
			get_cpu_input(&snake, &apple, UPPER_BOUND_X,
				      UPPER_BOUND_Y, LOWER_BOUND_X,
				      LOWER_BOUND_Y, &running);
		else
			get_user_input(&snake, &running);

		SDL_FRect new_head = *get_segment(&snake, 0);
		switch (snake.direction) {
		case UP:
			new_head.y -= CELL_SIZE;
			break;
		case DOWN:
			new_head.y += CELL_SIZE;
			break;
		case LEFT:
			new_head.x -= CELL_SIZE;
			break;
		case RIGHT:
			new_head.x += CELL_SIZE;
			break;
		}

		/* Snake-apple collision detection */
		bool ate_apple = false;
		if (new_head.x == apple.x && new_head.y == apple.y) {
			ate_apple = true;
			get_rand_pos(&(apple.x), &(apple.y), &snake);
		}

		/* Snake-border collision detection */
		if (new_head.x == LOWER_BOUND_X ||
		    new_head.x == UPPER_BOUND_X ||
		    new_head.y == LOWER_BOUND_Y || new_head.y == UPPER_BOUND_X)
			game_over_screen(renderer);

		/* Update snake body */
		push_back(&snake, new_head);
		if (!ate_apple)
			pop_back(
				&snake); /* remove back segment if not growing */

		/* Snake-snake collision detection */
		for (int i = 1; i < snake.size; i++) {
			SDL_FRect *segment = get_segment(&snake, i);
			if (new_head.x == segment->x &&
			    new_head.y == segment->y)
				game_over_screen(renderer);
		}

		/* Clear renderer */
		SDL_SetRenderDrawColor(renderer, BLACK);
		SDL_RenderClear(renderer);

		/* Render snake */
		SDL_SetRenderDrawColor(renderer, WHITE);
		for (int i = 0; i < snake.size; i++) {
			SDL_RenderFillRect(renderer, get_segment(&snake, i));
		}

		/* Render apple */
		SDL_SetRenderDrawColor(renderer, RED);
		SDL_RenderFillRect(renderer, &apple);

		/* Present rendered frame */
		SDL_RenderPresent(renderer);
		SDL_Delay(delay);
	}

	/* Free SDL resources */
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
