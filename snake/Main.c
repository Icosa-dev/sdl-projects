/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: MIT
 */

/*
 * Main.c - Entry point for the game
 *
 * A basic snake game implemented in SDL3. Keybinds are WASD and can be
 * changed by updating the KEYBIND macros. If you want the CPU to play for
 * you pass the argument "snake cpu".
 */

#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Snake.h"
#include "Input.h"
#include "Definitions.h"

// Window values
#define TITLE "Snake"
#define WINDOW_WIDTH (COLUMNS * CELL_SIZE)
#define WINDOW_HEIGHT (ROWS * CELL_SIZE)
#define UPPER_BOUND_X WINDOW_WIDTH
#define UPPER_BOUND_Y WINDOW_HEIGHT
#define LOWER_BOUND_X (-CELL_SIZE)
#define LOWER_BOUND_Y (-CELL_SIZE)

// String comparison macro for argparsing
#define STREQ(str1, str2) (strcmp(str1, str2) == 0)

// Generate random possition values for apple generation
float GetRandomX()
{
    return (float)(SDL_rand(COLUMNS) * CELL_SIZE);
}

float GetRandomY()
{
    return (float)(SDL_rand(ROWS) * CELL_SIZE);
}

// Freeze the screen when game over
void GameOverScreen(SDL_Renderer *renderer)
{
    SDL_Event event;
    for (;;)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                SDL_Quit();
                exit(0);
            }
        }
    }
}

int main(int argc, char **argv)
{
    bool CPUEnabled = false;

    if (argc > 1)
    {
        if (STREQ(argv[1], "help"))
        {
            printf("Usage: snake <OPTIONS>\n");
            printf("Options:\n");
            printf("\thelp:\t\tPrint help message\n");
            printf("\tcpu:\t\tHave the computer play for you\n");
        }
        else if (STREQ(argv[1], "cpu"))
        {
            CPUEnabled = true;
        }
    }

    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    // Create window and renderer
    SDL_Window *window = SDL_CreateWindow(TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    // Create snake
    Snake snake;
    InitSnake(&snake);
    // Push the first segment of the snake with possition in the middle of the window
    PushBack(&snake, (SDL_FRect){(float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT / 2, CELL_SIZE, CELL_SIZE});

    // Create apple
    SDL_FRect apple = (SDL_FRect){GetRandomX(), GetRandomY(), CELL_SIZE, CELL_SIZE};

    // Game loop
    bool running = true;
    while (running)
    {
        // Event handling
        GetInput(&snake, &running);

        SDL_FRect newHead = *GetSegment(&snake, 0);
        switch (snake.direction)
        {
        case UP:
            newHead.y -= CELL_SIZE;
            break;
        case DOWN:
            newHead.y += CELL_SIZE;
            break;
        case LEFT:
            newHead.x -= CELL_SIZE;
            break;
        case RIGHT:
            newHead.x += CELL_SIZE;
            break;
        }

        // Snake-apple collision detection
        bool ateApple = false;
        if (newHead.x == apple.x && newHead.y == apple.y)
        {
            ateApple = true;

            // NOTE: This does not check if the apple generates in the snake body
            apple.x = GetRandomX();
            apple.y = GetRandomY();
        }

        // Snake-border collision detection
        if (newHead.x == LOWER_BOUND_X || newHead.x == UPPER_BOUND_X || newHead.y == LOWER_BOUND_Y ||
            newHead.y == UPPER_BOUND_X)
            GameOverScreen(renderer);

        // Update snake body
        PushBack(&snake, newHead);
        if (!ateApple)
            PopBack(&snake); // remove back segment if not growing

        // Snake-snake collision detection
        for (int i = 1; i < snake.size; i++)
        {
            SDL_FRect *segment = GetSegment(&snake, i);
            if (newHead.x == segment->x && newHead.y == segment->y)
                GameOverScreen(renderer);
        }

        // Clear renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render snake
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int i = 0; i < snake.size; i++)
        {
            SDL_RenderFillRect(renderer, GetSegment(&snake, i));
        }

        // Render apple
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &apple);

        // Present rendered frame
        SDL_RenderPresent(renderer);
        SDL_Delay(DELAY);
    }

    // Free SDL resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
