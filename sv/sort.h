/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef SORT_H
#define SORT_H

#include <stdbool.h>
typedef struct SDL_Renderer SDL_Renderer;

/**
 * @brief Display an array as a bar graph
 *
 * @param renderer The renderer to draw the graph too
 * @param array The underlying data as an array
 * @param arraySize The size of the array
 * @param columnWidth The width of a bar column
 * @param windowHeight The height of the window to display to
 * @param delay How many milliseconds to delay after displaying
 * @param sorted If true renderer bars as green, else white
 */
extern void displayArray(SDL_Renderer *renderer, int array[], int array_size,
                         int column_width, int window_height, int delay,
                         bool sorted);

/**
 * @brief Sort and display an array using the bubble sort algorithm
 *
 * @param renderer The renderer to draw the graph too
 * @param array The underlying data as an array
 * @param arraySize The size of the array
 * @param columnWidth The width of a bar column
 * @param windowHeight The height of the window to display to
 * @param delay How many milliseconds to delay after displaying
 */
extern void bubbleSort(SDL_Renderer *renderer, int array[], int array_size,
                       int column_width, int window_height, int delay);

/**
 * @brief Sort and display an array using the selection sort algorithm
 *
 * @param renderer The renderer to draw the graph too
 * @param array The underlying data as an array
 * @param arraySize The size of the array
 * @param columnWidth The width of a bar column
 * @param windowHeight The height of the window to display to
 * @param delay How many milliseconds to delay after displaying
 */
extern void selectionSort(SDL_Renderer *renderer, int array[], int array_size,
                          int column_width, int window_height, int delay);

/**
 * @brief Sort and display an array using the insertion sort algorithm
 *
 * @param renderer The renderer to draw the graph too
 * @param array The underlying data as an array
 * @param arraySize The size of the array
 * @param columnWidth The width of a bar column
 * @param windowHeight The height of the window to display to
 * @param delay How many milliseconds to delay after displaying
 */
extern void insertionSort(SDL_Renderer *renderer, int array[], int array_size,
                          int column_width, int window_height, int delay);

/**
 * @brief Sort and display an array using the quicksort algorithm
 *
 * @param renderer The renderer to draw the graph too
 * @param array The underlying data as an array
 * @param arraySize The size of the array
 * @param low The low of the pivot
 * @param high The high of the pivot
 * @param columnWidth The width of a bar column
 * @param windowHeight The height of the window to display to
 * @param delay How many milliseconds to delay after displaying
 */
extern void quicksort(SDL_Renderer *renderer, int array[], int array_size,
                      int low, int high, int column_width, int window_height,
                      int delay);

typedef enum { BUBBLE, SELECTION, INSERTION, QUICK } Algorithm;

#endif
