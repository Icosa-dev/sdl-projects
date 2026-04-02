/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>
#include <stdlib.h>
#include <time.h>

#define CELL_SIZE 10
#define ARRAY_SIZE 50
#define COLUMNS ARRAY_SIZE
#define ROWS ARRAY_SIZE

#define TITLE "Sort Visualizer"
#define WINDOW_WIDTH (COLUMNS * CELL_SIZE)
#define WINDOW_HEIGHT (ROWS * CELL_SIZE)

#define DELAY 25

void GenerateRandomArray(int array[ARRAY_SIZE])
{
    srand(time(NULL));

    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        array[i] = i + 1;
    }

    for (int i = ARRAY_SIZE - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void DisplayArray(SDL_Renderer *renderer, int array[ARRAY_SIZE])
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        SDL_FRect rect = {
            (float)i * CELL_SIZE,
            (float)WINDOW_HEIGHT - (array[i] * CELL_SIZE),
            (float)CELL_SIZE - 1,
            (float)array[i] * CELL_SIZE
        };
        SDL_RenderFillRect(renderer, &rect);
    }
    
    SDL_RenderPresent(renderer);
}

void BubbleSort(SDL_Renderer *renderer, int array[ARRAY_SIZE])
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        for (int j = 0; j < ARRAY_SIZE - i - 1; j++)
        {
            if (array[j] > array[j + 1])
            {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;

                DisplayArray(renderer, array);
                SDL_Delay(DELAY);
            }
        }
    }
}

int main(void)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    int array[ARRAY_SIZE];
    GenerateRandomArray(array);

    bool running = true;
    bool sorted = false;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        if (!sorted)
        {
            BubbleSort(renderer, array);
            sorted = true;
        }

        DisplayArray(renderer, array);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
