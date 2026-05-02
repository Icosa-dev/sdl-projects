/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INPUT_H
#define INPUT_H

#include <SDL3/SDL.h>
#include <snake.h>

typedef struct {
    SDL_Keycode up, down, left, right;
} Keybinds;

#define WASD                                                                   \
    (Keybinds) { SDLK_W, SDLK_S, SDLK_A, SDLK_D }
#define ARROWS                                                                 \
    (Keybinds) { SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT }

/**
 * @brief Get the user input object
 *
 * @param snake
 * @param keybinds
 * @param running
 */
extern void getUserInput(Snake *snake, const Keybinds *keybinds, bool *running);

/**
 * @brief Get the cpu input object
 *
 * @param snake
 * @param apple
 * @param running
 * @param cellSize
 * @param windowWidth
 * @param windowHeight
 */
extern void getCPUInput(Snake *snake, bool *running, int cellSize,
                        int windowWidth, int windowHeight);

#endif
