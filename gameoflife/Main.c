/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: MIT
 *
 * Main.c - Entry point for the game
 *
 * An implementation of Conway's Game of Life with SDL3 in C. The board
 * is randomly generated each time and the generation count is printed to the console.
 */

#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "Definitions.h"
#include "Grid.h"

// Window values
#define TITLE "Game of Life"
#define WINDOW_WIDTH (COLUMNS * CELL_SIZE)
#define WINDOW_HEIGHT (ROWS * CELL_SIZE)

int main(void)
{
    // Initialize SDL with only video
    SDL_Init(SDL_INIT_VIDEO);

    // Initialize SDL window and renderer
    SDL_Window *window = SDL_CreateWindow(TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    // Randomize the grid of cells
    Grid grid;
    GenerateRandomGrid(grid);

    // Event loop
    bool running = true;
    SDL_Event event;
    int generationCount = 1;
    while (running)
    {
        // Check if the user quit
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        // Update the grid
        UpdateGrid(grid);

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, BLACK);
        SDL_RenderClear(renderer);

        // Draw the grid
        SDL_SetRenderDrawColor(renderer, WHITE);
        for (int r = 0; r < ROWS; r++)
        {
            for (int c = 0; c < COLUMNS; c++)
            {
                float x = c * CELL_SIZE;
                float y = r * CELL_SIZE;

                // Only draw alive cells with white
                if (grid[r][c] == ALIVE)
                {
                    SDL_RenderFillRect(renderer, &(SDL_FRect){x, y, CELL_SIZE, CELL_SIZE});
                }
            }
        }

        // Print generation count info 
        printf("Generation: %d\n", generationCount);
        fflush(stdout);
        generationCount++;

        // Present the rendered frame 
        SDL_RenderPresent(renderer);
    }

    // Free SDL resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
