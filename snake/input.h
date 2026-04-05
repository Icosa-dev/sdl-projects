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

#include "config.h"

typedef struct SDL_FRect SDL_FRect;
struct snake;

extern void get_user_input(struct snake *snake, const struct config *config,
	bool *running);
extern void get_cpu_input(struct snake *snake, const SDL_FRect *apple,
	const struct config *config, bool *running);

#endif
