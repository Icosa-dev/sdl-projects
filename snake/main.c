/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
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

#define STREQ(str1, str2) (strcmp(str1, str2) == 0)

static void printVersion(void) {
    printf("%s: %s (%s)\n", PROGRAM_NAME, DESCRIPTION, VERSION);
    exit(0);
}

static void printHelp(void) {
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
    printf("\t--delay <int>\t\t\t\t\tSet the delay of the program in "
           "milliseconds\n");
    exit(0);
}

static void freezeScreen(void) {
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

/**
 * @brief Set the x and y value of the rect to a random x and y not inside of
 * the snake body
 *
 * @param rect Input rect
 * @param snake The snake to avoid
 * @param max_x The max x value
 * @param max_y The max y value
 */
static void getRandPos(SDL_FRect *rect, Snake *snake, int cell_size, int rows,
                       int columns) {
    bool overlapping;
    do {
        overlapping = false;

        rect->x = (float)(SDL_rand(columns) * cell_size);
        rect->y = (float)(SDL_rand(rows) * cell_size);

        for (int i = 0; i < snake->size; i++) {
            SDL_FRect *seg = snakeGetSegment(snake, i);

            if (rect->x == seg->x && rect->y == seg->y) {
                overlapping = true;
                break;
            }
        }
    } while (overlapping);
}

int main(int argc, char **argv) {
    // Default game values
    int cellSize = 25;
    int rows = 20;
    int columns = 20;

    Keybinds keybinds = WASD;

    bool cpu_enabled = false;
    int delay = 50;

    // Argument parsing
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            const char *arg = argv[i];

            // Flag parsing
            if (STREQ(arg, "-v") || STREQ(arg, "--version"))
                printVersion();
            else if (STREQ(arg, "-h") || STREQ(arg, "--help"))
                printHelp();
            else if (STREQ(arg, "--cpu-enabled"))
                cpu_enabled = true;

            // Optional arg parsing
            if (i + 1 < argc) {
                const char *value = argv[i + 1];

                if (STREQ(arg, "--cell-size"))
                    cellSize = atoi(value);
                else if (STREQ(arg, "--rows"))
                    rows = atoi(value);
                else if (STREQ(arg, "--columns"))
                    columns = atoi(value);
                else if (STREQ(arg, "--keybinds")) {
                    if (STREQ(value, "arrows")) {
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

    int snakeMaxSize = rows * columns;

    int windowWidth = columns * cellSize;
    int windowHeight = rows * cellSize;

    /* Game logic */
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    SDL_Window *window =
        SDL_CreateWindow(PROGRAM_NAME, windowWidth, windowHeight, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    Snake *snake = snakeInit();
    float centerX = (float)windowWidth / 2;
    float centerY = (float)windowHeight / 2;
    snakePushFront(snake, (SDL_FRect){centerX, centerY, cellSize, cellSize});

    SDL_FRect apple = (SDL_FRect){0, 0, cellSize, cellSize};
    getRandPos(&apple, snake, cellSize, rows, columns);

    bool running = true;
    while (running) {
        /* Input handling */
        if (cpu_enabled)
            getCPUInput(snake, &running, cellSize, windowHeight, windowWidth);
        else
            getUserInput(snake, &keybinds, &running);

        /* Move snake */
        SDL_FRect new_head = snake->head->rect;
        switch (snake->direction) {
        case UP:
            new_head.y -= cellSize;
            break;
        case DOWN:
            new_head.y += cellSize;
            break;
        case LEFT:
            new_head.x -= cellSize;
            break;
        case RIGHT:
            new_head.x += cellSize;
            break;
        }

        /* Snake-apple collision detection */
        bool ate_apple = false;
        if (new_head.x == apple.x && new_head.y == apple.y) {
            ate_apple = true;
            getRandPos(&apple, snake, cellSize, rows, columns);
        }

        /* Snake-border collision detection */
        if (new_head.x < 0 || new_head.x == windowWidth || new_head.y < 0 ||
            new_head.y == windowHeight)
            freezeScreen();

        /* Update snake body */
        snakePushFront(snake, new_head);
        if (!ate_apple)
            snakePopBack(snake); /* remove back segment if not growing */

        /* Snake-snake collision detection */
        for (int i = 1; i < snake->size; i++) {
            SDL_FRect *segment = snakeGetSegment(snake, i);
            if (new_head.x == segment->x && new_head.y == segment->y)
                freezeScreen();
        }

        if (snake->size == snakeMaxSize)
            freezeScreen();

        /* Clear screen */
        SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(BLACK));
        SDL_RenderClear(renderer);

        /* Render apple */
        SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(RED));
        SDL_RenderFillRect(renderer, &apple);

        /* Render snake */
        SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(WHITE));
        for (int i = 0; i < snake->size; i++) {
            SDL_RenderFillRect(renderer, snakeGetSegment(snake, i));
        }

        /* Present renderer */
        SDL_RenderPresent(renderer);

        SDL_Delay(delay);
    }

    snakeFree(snake);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
