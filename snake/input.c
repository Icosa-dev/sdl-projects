/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <SDL3/SDL.h>

#include "input.h"
#include "snake.h"

void getUserInput(Snake *snake, const Keybinds *keybinds, bool *running) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT)
            *running = false;

        if (event.type == SDL_EVENT_KEY_DOWN) {
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
 * @param cellSize The size of a grid cell on the board
 * @param windowWidth The width of the playable window
 * @param windowHeight The height of the playable window
 * @return Direction The direction the snake should go to go on the longest
 * possible path
 */
static Direction getLongestDirection(const Snake *snake, int cell_size,
                                     int window_width, int window_height) {
    int ix = (snake->head->rect.x / cell_size);
    int iy = (snake->head->rect.y / cell_size);
    int gridW = (window_width / cell_size);
    int gridH = (window_height / cell_size);

    if (ix == 0 && iy > 0) {
        return UP;
    }

    if (iy == 0 && ix < gridW - 1) {
        return RIGHT;
    }

    if (ix == gridW - 1 && iy % 2 == 0) {
        return DOWN;
    }
    if (ix == 1 && iy % 2 != 0 && iy < gridH - 1) {
        return DOWN;
    }

    if (iy == gridH - 1 && ix > 0) {
        return LEFT;
    }

    if (iy % 2 == 0) {
        return RIGHT;
    } else {
        return LEFT;
    }
}

/*
 * Finally! The first snake game to be fully AI powered!
 * Truely the future of software!
 */
void getCPUInput(Snake *snake, bool *running, int cellSize, int windowWidth,
                 int windowHeight) {
    /* Check if user quit */
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT)
            *running = false;
    }

    snake->direction =
        getLongestDirection(snake, cellSize, windowWidth, windowHeight);
}
