/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef SORT_H_
#define SORT_H_

#include <stdbool.h>
typedef struct SDL_Renderer SDL_Renderer;

/**
 * @brief Display an array as a bar graph
 *
 * @param renderer The renderer to draw the graph too
 * @param array The underlying data as an array
 * @param array_size The size of the array
 * @param column_width The width of a bar column
 * @param window_height The height of the window to display to
 * @param delay How many milliseconds to delay after displaying
 * @param sorted If true renderer bars as green, else white
 */
extern void display_array(SDL_Renderer *renderer, uint32_t array[],
	size_t array_size, uint32_t column_width, uint32_t window_height, uint32_t delay,
	bool sorted);

/**
 * @brief Sort and display an array using the bubble sort algorithm
 *
 * @param renderer The renderer to draw the graph too
 * @param array The underlying data as an array
 * @param array_size The size of the array
 * @param column_width The width of a bar column
 * @param window_height The height of the window to display to
 * @param delay How many milliseconds to delay after displaying
 */
extern void bubble_sort(SDL_Renderer *renderer, uint32_t array[], size_t array_size,
	uint32_t column_width, uint32_t window_height, uint32_t delay);

/**
 * @brief Sort and display an array using the selection sort algorithm
 *
 * @param renderer The renderer to draw the graph too
 * @param array The underlying data as an array
 * @param array_size The size of the array
 * @param column_width The width of a bar column
 * @param window_height The height of the window to display to
 * @param delay How many milliseconds to delay after displaying
 */
extern void selection_sort(SDL_Renderer *renderer, uint32_t array[],
	size_t array_size, uint32_t column_width, uint32_t window_height, uint32_t delay);

/**
 * @brief Sort and display an array using the insertion sort algorithm
 *
 * @param renderer The renderer to draw the graph too
 * @param array The underlying data as an array
 * @param array_size The size of the array
 * @param column_width The width of a bar column
 * @param window_height The height of the window to display to
 * @param delay How many milliseconds to delay after displaying
 */
extern void insertion_sort(SDL_Renderer *renderer, uint32_t array[],
	size_t array_size, uint32_t column_width, uint32_t window_height, uint32_t delay);

/**
 * @brief Sort and display an array using the quicksort algorithm
 *
 * @param renderer The renderer to draw the graph too
 * @param array The underlying data as an array
 * @param array_size The size of the array
 * @param column_width The width of a bar column
 * @param window_height The height of the window to display to
 * @param delay How many milliseconds to delay after displaying
 */
extern void quicksort(SDL_Renderer *renderer, uint32_t array[], size_t array_size,
	size_t low, size_t high, uint32_t column_width, uint32_t window_height,
	uint32_t delay);

enum algorithm
{
	BUBBLE,
	SELECTION,
	INSERTION,
	QUICK
};

#endif
