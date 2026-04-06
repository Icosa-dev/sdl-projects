/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: MIT
 */

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/color.h"

#define CELL_SIZE 5
int array_size = 201;
// I need array size to be mutable but I dont feel like changing the name
#define ARRAY_SIZE array_size
#define COLUMNS ARRAY_SIZE
#define ROWS ARRAY_SIZE

#define TITLE "Sort Visualizer"
#define WINDOW_WIDTH (COLUMNS * CELL_SIZE)
#define WINDOW_HEIGHT (ROWS * CELL_SIZE)

#define STREQ(str1, str2) (strcmp(str1, str2) == 0)

// same as above with array size
int delay = 25;
#define DELAY delay

/* Generate a random array to sort */
static void
generate_random_array(int arr[ARRAY_SIZE])
{
	srand(time(NULL));

	/*
	 * Fill the array with sorted values. The array
	 * that is generated will have 1 of each value
	 * from 1-50.
	 */
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		arr[i] = i + 1;
	}

	/* Shuffle the array */
	for (int i = ARRAY_SIZE - 1; i > 0; i--)
	{
		int j = rand() % (i + 1);
		int temp = arr[i];
		arr[i] = arr[j];
		arr[j] = temp;
	}
}

/* Display the array as a bar graph on the screen */
static void
display_array(SDL_Renderer *renderer, int arr[ARRAY_SIZE], bool sorted)
{
	SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(BLACK));
	SDL_RenderClear(renderer);

	if (sorted)
		SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(GREEN));
	else
		SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(WHITE));
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		SDL_FRect rect = { (float)i * CELL_SIZE,
			(float)WINDOW_HEIGHT - (arr[i] * CELL_SIZE), (float)CELL_SIZE - 1,
			(float)arr[i] * CELL_SIZE };
		SDL_RenderFillRect(renderer, &rect);
	}

	SDL_RenderPresent(renderer);
}

/* Check if the user quit */
static void
check_event()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
		if (event.type == SDL_EVENT_QUIT)
		{
			SDL_Quit();
			exit(0);
		}
}

/* Sorting algorithms */
static void
bubble_sort(SDL_Renderer *renderer, int arr[ARRAY_SIZE])
{
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		for (int j = 0; j < ARRAY_SIZE - i - 1; j++)
		{
			check_event();
			if (arr[j] > arr[j + 1])
			{
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;

				display_array(renderer, arr, false);
				SDL_Delay(DELAY);
			}
		}
	}
}

static void
selection_sort(SDL_Renderer *renderer, int arr[ARRAY_SIZE])
{
	for (int i = 0; i < ARRAY_SIZE - 1; i++)
	{
		int min_idx = i;
		for (int j = i + 1; j < ARRAY_SIZE; j++)
		{
			check_event();
			if (arr[j] < arr[min_idx])
			{
				min_idx = j;
			}
		}
		int temp = arr[min_idx];
		arr[min_idx] = arr[i];
		arr[i] = temp;

		display_array(renderer, arr, false);
		SDL_Delay(DELAY);
	}
}

static void
insertion_sort(SDL_Renderer *renderer, int arr[ARRAY_SIZE])
{
	for (int i = 1; i < ARRAY_SIZE; i++)
	{
		int key = arr[i];
		int j = i - 1;

		while (j >= 0 && arr[j] > key)
		{
			check_event();

			arr[j + 1] = arr[j];
			j = j - 1;

			display_array(renderer, arr, false);
			SDL_Delay(DELAY / 2);
		}
		arr[j + 1] = key;

		display_array(renderer, arr, false);
		SDL_Delay(DELAY);
	}
}

static int
partition(SDL_Renderer *renderer, int array[], int low, int high)
{
	int pivot = array[high];
	int i = (low - 1);

	for (int j = low; j <= high - 1; j++)
	{
		check_event();

		if (array[j] < pivot)
		{
			i++;
			int temp = array[i];
			array[i] = array[j];
			array[j] = temp;

			display_array(renderer, array, false);
			SDL_Delay(DELAY);
		}
	}
	int temp = array[i + 1];
	array[i + 1] = array[high];
	array[high] = temp;

	display_array(renderer, array, false);
	SDL_Delay(DELAY);
	return (i + 1);
}

static void
quicksort(SDL_Renderer *renderer, int array[], int low, int high)
{
	if (low < high)
	{
		int pi = partition(renderer, array, low, high);
		quicksort(renderer, array, low, pi - 1);
		quicksort(renderer, array, pi + 1, high);
	}
}

/* Enum to select which algorithm to display */
enum algorithm
{
	BUBBLE_SORT,
	SELECTION_SORT,
	INSERTION_SORT,
	QUICKSORT
};

int
main(int argc, char **argv)
{
	enum algorithm selected_algorithm;

	/* Check the user input for which algorthim to use */
	if (argc > 1)
	{
		if (STREQ(argv[1], "bubble"))
			selected_algorithm = BUBBLE_SORT;
		else if (STREQ(argv[1], "selection"))
			selected_algorithm = SELECTION_SORT;
		else if (STREQ(argv[1], "insertion"))
			selected_algorithm = INSERTION_SORT;
		else if (STREQ(argv[1], "quick"))
			selected_algorithm = QUICKSORT;
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
	} else
		selected_algorithm = BUBBLE_SORT; /* default to bubble */

	/* SDL initialize with video only */
	SDL_Init(SDL_INIT_VIDEO);

	/* Create SDL window and renderer */
	SDL_Window *window = SDL_CreateWindow(TITLE, WINDOW_WIDTH, WINDOW_HEIGHT,
		0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

	/* Generate random array to sort */
	int array[ARRAY_SIZE];
	generate_random_array(array);

	/* Event loop */
	bool running = true;
	bool sorted = false;
	SDL_Event event;
	while (running)
	{
		check_event();

		if (!sorted)
		{
			switch (selected_algorithm)
			{
			case BUBBLE_SORT:
				bubble_sort(renderer, array);
				break;
			case SELECTION_SORT:
				selection_sort(renderer, array);
				break;
			case INSERTION_SORT:
				insertion_sort(renderer, array);
				break;
			case QUICKSORT:
				quicksort(renderer, array, 0, ARRAY_SIZE);
				break;
			}
			sorted = true;
		}

		display_array(renderer, array, sorted);
	}

	/* Free SDL resources */
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
