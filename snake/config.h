/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <SDL3/SDL.h>

#include "../include/color.h"

/* Configuration variables for the game */
struct config
{
	int cell_size, rows, columns;

	const char *title;
	int window_width, window_height;

	struct color board_color, snake_color, apple_color;

	unsigned int keybind_up, keybind_down, keybind_left, keybind_right;

	bool cpu_enabled;

	int delay;
};

#define STD_CONFIG                                                          \
	{ 25, 20, 20, "Snake", (25 * 20), (25 * 20), BLACK, WHITE, RED, SDLK_W, \
		SDLK_S, SDLK_A, SDLK_D, false, 50 }

#endif
