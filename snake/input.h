/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef INPUT_H_
#define INPUT_H_

#include <SDL3/SDL.h>

struct keybinds
{
	SDL_Keycode up, down, left, right;
};

#define WASD                           \
	(struct keybinds)                  \
	{                                  \
		SDLK_W, SDLK_S, SDLK_A, SDLK_D \
	}
#define ARROWS                                    \
	(struct keybinds)                             \
	{                                             \
		SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT \
	}

struct snake;

/**
 * @brief Get the user input object
 * 
 * @param snake 
 * @param keybinds 
 * @param running 
 */
extern void get_user_input(struct snake *snake, const struct keybinds *keybinds, bool *running);

/**
 * @brief Get the cpu input object
 * 
 * @param snake 
 * @param apple 
 * @param running 
 * @param cell_size 
 * @param window_width 
 * @param window_height 
 */
extern void get_cpu_input(struct snake *snake, const SDL_FRect *apple, bool *running, int cell_size, int window_width, int window_height);

#endif
