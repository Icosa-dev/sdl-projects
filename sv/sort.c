/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <SDL3/SDL.h>
#include <color.h>
#include <stdlib.h>

#include "sort.h"

// Display the array as a bar graph on the screen
void displayArray(SDL_Renderer *renderer, int array[], int array_size,
                  int column_width, int window_height, int delay, bool sorted) {
    SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(BLACK));
    SDL_RenderClear(renderer);

    if (sorted)
        SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(GREEN));
    else
        SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(WHITE));
    for (int i = 0; i < array_size; i++) {
        SDL_FRect rect = {.x = (float)i * column_width,
                          .y = (float)window_height - (array[i] * column_width),
                          .w = (float)column_width - 1,
                          .h = (float)array[i] * column_width};
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(delay);
}

// Check if the user quit
static void checkEvent(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event))
        if (event.type == SDL_EVENT_QUIT) {
            SDL_Quit();
            exit(0);
        }
}

void bubbleSort(SDL_Renderer *renderer, int array[], int array_size,
                int column_width, int window_height, int delay) {
    for (int i = 0; i < array_size; i++) {
        for (int j = 0; j < array_size - i - 1; j++) {
            checkEvent();
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;

                displayArray(renderer, array, array_size, column_width,
                             window_height, delay, false);
            }
        }
    }
}

void selectionSort(SDL_Renderer *renderer, int array[], int array_size,
                   int column_width, int window_height, int delay) {
    for (int i = 0; i < array_size - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < array_size; j++) {
            checkEvent();
            if (array[j] < array[min_idx]) {
                min_idx = j;
            }
        }
        int temp = array[min_idx];
        array[min_idx] = array[i];
        array[i] = temp;

        displayArray(renderer, array, array_size, column_width, window_height,
                     delay, false);
    }
}

void insertionSort(SDL_Renderer *renderer, int array[], int array_size,
                   int column_width, int window_height, int delay) {
    for (int i = 1; i < array_size; i++) {
        int key = array[i];
        int j = i - 1;

        while (array[j] > key) {
            checkEvent();

            array[j + 1] = array[j];
            j = j - 1;

            displayArray(renderer, array, array_size, column_width,
                         window_height, delay, false);
        }
        array[j + 1] = key;

        displayArray(renderer, array, array_size, column_width, window_height,
                     delay, false);
    }
}

static int partition(SDL_Renderer *renderer, int array[], int array_size,
                     int low, int high, int column_width, int window_height,
                     int delay) {
    int pivot = array[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        checkEvent();

        if (array[j] < pivot) {
            i++;
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;

            displayArray(renderer, array, array_size, column_width,
                         window_height, delay, false);
        }
    }
    int temp = array[i + 1];
    array[i + 1] = array[high];
    array[high] = temp;

    displayArray(renderer, array, array_size, column_width, window_height,
                 delay, false);
    return (i + 1);
}

void quicksort(SDL_Renderer *renderer, int array[], int arraySize, int low,
               int high, int columnWidth, int windowHeight, int delay) {
    if (low < high) {
        size_t pi = partition(renderer, array, arraySize, low, high,
                              columnWidth, windowHeight, delay);
        quicksort(renderer, array, arraySize, low, pi - 1, columnWidth,
                  windowHeight, delay);
        quicksort(renderer, array, arraySize, pi + 1, high, columnWidth,
                  windowHeight, delay);
    }
}
