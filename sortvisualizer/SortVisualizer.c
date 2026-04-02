/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: MIT
 */

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gamepad.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

/*
 * SortVisualizer.c
 *
 * A sorting algorithm visualizer. For more information run "sortvisualizer help"
 */

// Grid values
#define CELL_SIZE 10
#define ARRAY_SIZE 50
#define COLUMNS ARRAY_SIZE
#define ROWS ARRAY_SIZE

// Window values
#define TITLE "Sort Visualizer"
#define WINDOW_WIDTH (COLUMNS * CELL_SIZE)
#define WINDOW_HEIGHT (ROWS * CELL_SIZE)

// String comparison macro
#define STREQ(str1, str2) (strcmp(str1, str2) == 0)

// Delay in milliseconds
#define DELAY 50

// Generate a random array to sort
void GenerateRandomArray(int array[ARRAY_SIZE])
{
    srand(time(NULL));

    // Fill the array with sorted values. The array 
    // that is generated will have 1 of each value
    // from 1-50.
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        array[i] = i + 1;
    }

    // Shuffle the array
    for (int i = ARRAY_SIZE - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

// Display the array as a bar graph on the screen
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

// Check if the user quit
void CheckEvent()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
        if (event.type == SDL_EVENT_QUIT) { SDL_Quit(); exit(0); }
}

// Sorting algorithms
void BubbleSort(SDL_Renderer *renderer, int array[ARRAY_SIZE])
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        for (int j = 0; j < ARRAY_SIZE - i - 1; j++)
        {
            CheckEvent();
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

void SelectionSort(SDL_Renderer *renderer, int array[ARRAY_SIZE])
{
    for (int i = 0; i < ARRAY_SIZE - 1; i++)
    {
        int min_idx = i;
        for (int j = i + 1; j < ARRAY_SIZE; j++)
        {
            CheckEvent();
            if (array[j] < array[min_idx])
            {
                min_idx = j;
            }
        }
        int temp = array[min_idx];
        array[min_idx] = array[i];
        array[i] = temp;

        DisplayArray(renderer, array);
        SDL_Delay(DELAY);
    }
}

void InsertionSort(SDL_Renderer *renderer, int array[ARRAY_SIZE])
{
    for (int i = 1; i < ARRAY_SIZE; i++)
    {
        int key = array[i];
        int j = i - 1;

        while (j >= 0 && array[j] > key)
        {
            CheckEvent();

            array[j + 1] = array[j];
            j = j - 1;
            
            DisplayArray(renderer, array);
            SDL_Delay(DELAY / 2); 
        }
        array[j + 1] = key;

        DisplayArray(renderer, array);
        SDL_Delay(DELAY);
    }
}

int Partition(SDL_Renderer *renderer, int array[], int low, int high)
{
    int pivot = array[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        CheckEvent();

        if (array[j] < pivot)
        {
            i++;
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;

            DisplayArray(renderer, array);
            SDL_Delay(DELAY);
        }
    }
    int temp = array[i + 1];
    array[i + 1] = array[high];
    array[high] = temp;

    DisplayArray(renderer, array);
    SDL_Delay(DELAY);
    return (i + 1);
}

void QuickSort(SDL_Renderer *renderer, int array[], int low, int high)
{
    if (low < high)
    {
        int pi = Partition(renderer, array, low, high);
        QuickSort(renderer, array, low, pi - 1);
        QuickSort(renderer, array, pi + 1, high);
    }
}

// Enum to select which algorithm to display
typedef enum
{
    BUBBLE_SORT,
    SELECTION_SORT,
    INSERTION_SORT,
    QUICKSORT
} Algorithm;

int main(int argc, char **argv)
{
    Algorithm algorithm;

    // Check the user input for which algorthim to use
    if (argc > 1)
    {
        if (STREQ(argv[1], "bubble"))
            algorithm = BUBBLE_SORT;
        else if (STREQ(argv[1], "selection"))
            algorithm = SELECTION_SORT;
        else if (STREQ(argv[1], "insertion"))
            algorithm = INSERTION_SORT;
        else if (STREQ(argv[1], "quick"))
            algorithm = QUICKSORT;
        else
        {
            // Help message if invalid input or "help"
            printf("Usage: sortvisualizer <OPTIONS>\n");
            printf("Options:\n");
            printf("\thelp\t\tPrint help message\n");
            printf("\tbubble\t\tUse bubble sort algorithm\n");
            printf("\tselection\tUse selection sort algorithm\n");
            printf("\tinsertion\tUse insertion sort algorthim\n");
            printf("\tquick\t\tUse quicksort algorithm\n");
            return 0;
        }
    }

    // SDL initialize with video only
    SDL_Init(SDL_INIT_VIDEO);

    // Create SDL window and renderer
    SDL_Window *window = SDL_CreateWindow(TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    // Generate random array to sort
    int array[ARRAY_SIZE];
    GenerateRandomArray(array);

    // Event loop
    bool running = true;
    bool sorted = false;
    SDL_Event event;
    while (running)
    {
        CheckEvent();

        if (!sorted)
        {
            switch (algorithm)
            {
            case BUBBLE_SORT:
                BubbleSort(renderer, array); break;
            case SELECTION_SORT:
                SelectionSort(renderer, array); break;
            case INSERTION_SORT:
                InsertionSort(renderer, array); break;
            case QUICKSORT:
                QuickSort(renderer, array, 0, ARRAY_SIZE); break;
            }
            sorted = true;
        }

        DisplayArray(renderer, array);
    }

    // Free SDL resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
