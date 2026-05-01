/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <SDL3/SDL.h>
#include <color.h>
#include <stdlib.h>

#include "sort.h"

/* Display the array as a bar graph on the screen */
void display_array(SDL_Renderer *renderer, uint32_t array[], size_t array_size,
                   uint32_t column_width, uint32_t window_height,
                   uint32_t delay, bool sorted) {
    SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(BLACK));
    SDL_RenderClear(renderer);

    if (sorted)
        SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(GREEN));
    else
        SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(WHITE));
    for (size_t i = 0; i < array_size; i++) {
        SDL_FRect rect = {.x = (float)i * column_width,
                          .y = (float)window_height - (array[i] * column_width),
                          .w = (float)column_width - 1,
                          .h = (float)array[i] * column_width};
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(delay);
}

/* Check if the user quit */
static void check_event(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event))
        if (event.type == SDL_EVENT_QUIT) {
            SDL_Quit();
            exit(0);
        }
}

void bubble_sort(SDL_Renderer *renderer, uint32_t array[], size_t array_size,
                 uint32_t column_width, uint32_t window_height,
                 uint32_t delay) {
    for (size_t i = 0; i < array_size; i++) {
        for (size_t j = 0; j < array_size - i - 1; j++) {
            check_event();
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;

                display_array(renderer, array, array_size, column_width,
                              window_height, delay, false);
            }
        }
    }
}

void selection_sort(SDL_Renderer *renderer, uint32_t array[], size_t array_size,
                    uint32_t column_width, uint32_t window_height,
                    uint32_t delay) {
    for (size_t i = 0; i < array_size - 1; i++) {
        size_t min_idx = i;
        for (size_t j = i + 1; j < array_size; j++) {
            check_event();
            if (array[j] < array[min_idx]) {
                min_idx = j;
            }
        }
        uint32_t temp = array[min_idx];
        array[min_idx] = array[i];
        array[i] = temp;

        display_array(renderer, array, array_size, column_width, window_height,
                      delay, false);
    }
}

void insertion_sort(SDL_Renderer *renderer, uint32_t array[], size_t array_size,
                    uint32_t column_width, uint32_t window_height,
                    uint32_t delay) {
    for (size_t i = 1; i < array_size; i++) {
        uint32_t key = array[i];
        size_t j = i - 1;

        while (array[j] > key) {
            check_event();

            array[j + 1] = array[j];
            j = j - 1;

            display_array(renderer, array, array_size, column_width,
                          window_height, delay, false);
        }
        array[j + 1] = key;

        display_array(renderer, array, array_size, column_width, window_height,
                      delay, false);
    }
}

static size_t partition(SDL_Renderer *renderer, uint32_t array[],
                        size_t array_size, size_t low, size_t high,
                        uint32_t column_width, uint32_t window_height,
                        uint32_t delay) {
    size_t pivot = array[high];
    size_t i = (low - 1);

    for (size_t j = low; j <= high - 1; j++) {
        check_event();

        if (array[j] < pivot) {
            i++;
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;

            display_array(renderer, array, array_size, column_width,
                          window_height, delay, false);
        }
    }
    uint32_t temp = array[i + 1];
    array[i + 1] = array[high];
    array[high] = temp;

    display_array(renderer, array, array_size, column_width, window_height,
                  delay, false);
    return (i + 1);
}

void quicksort(SDL_Renderer *renderer, uint32_t array[], size_t array_size,
               size_t low, size_t high, uint32_t cell_size,
               uint32_t window_height, uint32_t delay) {
    if (low < high) {
        size_t pi = partition(renderer, array, array_size, low, high, cell_size,
                              window_height, delay);
        quicksort(renderer, array, array_size, low, pi - 1, cell_size,
                  window_height, delay);
        quicksort(renderer, array, array_size, pi + 1, high, cell_size,
                  window_height, delay);
    }
}
