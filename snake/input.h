/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * input.h - Functionality for getting input from the user or CPU
 */

#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

/* Keybinds. Default is WASD. */
#define KEYBIND_UP SDLK_W
#define KEYBIND_DOWN SDLK_S
#define KEYBIND_LEFT SDLK_A
#define KEYBIND_RIGHT SDLK_D

typedef struct SDL_FRect SDL_FRect;
struct snake;

extern void get_user_input(struct snake *snake, bool *running);
extern void get_cpu_input(struct snake *snake, const SDL_FRect *apple,
			  float upper_bound_x, float upper_bound_y,
			  float lower_bound_x, float lower_bound_y,
			  bool *running);

#endif
