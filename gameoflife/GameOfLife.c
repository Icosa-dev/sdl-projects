/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: Apache-2.0
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

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
                continue;

            int row = r + i;
            int col = c + j;

            if (row >= 0 && row < ROWS && col >= 0 && col < COLUMNS)
            {
                if (board[row][col] == ALIVE)
                    count++;
            }
        }
    }

    return count;
}

void UpdateBoard(bool board[ROWS][COLUMNS])
{
    bool tempBoard[ROWS][COLUMNS];

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLUMNS; c++)
        {
            int count = CountNeighbors(board, r, c);
            bool currentCell = board[r][c];

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

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLUMNS; c++)
        {
            board[r][c] = tempBoard[r][c];
        }
    }
}

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
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    // board of cells
    bool board[ROWS][COLUMNS];
    RandomizeBoard(board);

    bool running = true;
    SDL_Event event;
    int generationCount = 1;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        UpdateBoard(board);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int r = 0; r < ROWS; r++)
        {
            for (int c = 0; c < COLUMNS; c++)
            {
                float x = c * CELL_SIZE;
                float y = r * CELL_SIZE;

                if (board[r][c] == ALIVE)
                {
                    SDL_RenderFillRect(renderer, &(SDL_FRect){x, y, CELL_SIZE, CELL_SIZE});
                }
            }
        }

        printf("Generation: %d\n", generationCount);
        fflush(stdout);
        generationCount++;
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
