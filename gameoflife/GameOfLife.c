/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <SDL3/SDL.h>

// Grid values
#define CELL_SIZE 15
#define ROWS 50
#define COLUMNS 50

// Window values
#define WINDOW_WIDTH (COLUMNS * CELL_SIZE)
#define WINDOW_HEIGHT (ROWS * CELL_SIZE)

int main(void)
{
    SDL_Init(SDL_INIT_VIDEO);
    return 0;
}
