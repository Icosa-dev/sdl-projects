/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: MIT
 */

#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

// Grid values
#define CELL_SIZE 1
#define ROWS 500
#define COLUMNS 500

// Window values
#define TITLE "Game of Life"
#define WINDOW_WIDTH (COLUMNS * CELL_SIZE)
#define WINDOW_HEIGHT (ROWS * CELL_SIZE)

// Alias true and false to ALIVE and DEAD
#define ALIVE true
#define DEAD false

int CountNeighbors(bool board[ROWS][COLUMNS], int r, int c)
{
    int count = 0;

    // Index around the cell (-1, 0, +1)
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            // If i and j are 0 then the current neighbor
            // being checked is the current cell. Skip.
            if (i == 0 && j == 0)
                continue;

            // The neighbor being checked is offset from the
            // current cell by i and j
            int row = r + i;
            int col = c + j;

            // if neighbor is not off the grid
            if (row >= 0 && row < ROWS && col >= 0 && col < COLUMNS)
            {
                // If the neighbor is alive add to count
                if (board[row][col] == ALIVE)
                    count++;
            }
        }
    }

    return count;
}

void UpdateBoard(bool board[ROWS][COLUMNS])
{
    // Hold next generation of cells
    bool tempBoard[ROWS][COLUMNS];

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLUMNS; c++)
        {
            int count = CountNeighbors(board, r, c);
            bool currentCell = board[r][c];

            /*
             * RULES:
             * 1) Death by Underpopulation: If the current cell is alive but has less than
             *    2 alive neighbors it dies as if by underpopulation.
             *
             * 2) Death by Overpopulation: If the current cell is alive but has more than
             *    3 alive neighbors it dies as if by overpopulation.
             *
             * 3) Reproduction: If the current cell is alive and has exactly 3 alive neighbors
             *    it becomes alive as if by reproduction.
             *
             * 4) Survival: If none of the above are met the state of the cell persists.
             */
            if (currentCell == ALIVE && count < 2)
                tempBoard[r][c] = false; // death by underpopulation
            else if (currentCell == ALIVE && count > 3)
                tempBoard[r][c] = false; // death by overpopulation
            else if (currentCell == DEAD && count == 3)
                tempBoard[r][c] = true; // reproduction
            else
                tempBoard[r][c] = currentCell; // survival
        }
    }

    // Copy the temporary board into the actual board
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLUMNS; c++)
        {
            board[r][c] = tempBoard[r][c];
        }
    }
}

// Generate a random board
void RandomizeBoard(bool board[ROWS][COLUMNS])
{
    srand(time(NULL));

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLUMNS; c++)
        {
            board[r][c] = rand() % 2;
        }
    }
}

int main(void)
{
    // Initialize SDL with only video
    SDL_Init(SDL_INIT_VIDEO);

    // Initialize SDL window and renderer
    SDL_Window *window = SDL_CreateWindow(TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    // Randomize the board of cells
    bool board[ROWS][COLUMNS];
    RandomizeBoard(board);

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

        // Update the board
        UpdateBoard(board);

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the board 
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int r = 0; r < ROWS; r++)
        {
            for (int c = 0; c < COLUMNS; c++)
            {
                float x = c * CELL_SIZE;
                float y = r * CELL_SIZE;

                // Only draw alive cells with white
                if (board[r][c] == ALIVE)
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
