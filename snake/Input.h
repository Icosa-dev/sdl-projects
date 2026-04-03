/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Input.h - Functionality for getting input from the user or CPU
 */

#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include "Snake.h"

// Keybinds. Default is WASD.
#define KEYBIND_UP SDLK_W
#define KEYBIND_DOWN SDLK_S
#define KEYBIND_LEFT SDLK_A
#define KEYBIND_RIGHT SDLK_D

extern void GetInput(Snake *snake, bool *input);

#endif
