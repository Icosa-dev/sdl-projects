/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: MIT
 */

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "sort.h"

#define PROGRAM_NAME "sv"
#define DESCRIPTION "A basic sorting algorithm visualizer implemented in SDL3"
#define AUTHORS "LJC"
#define VERSION "v0.1.0"
#define MIT_LICENSE_TEXT                                                                \
	"Copyright (c) " __DATE__ " " AUTHORS "\n\n"                                        \
	"Permission is hereby granted, free of charge, to any person obtaining a copy\n"    \
	"of this software and associated documentation files (the \"Software\"), to deal\n" \
	"in the Software without restriction, including without limitation the rights\n"    \
	"to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"       \
	"copies of the Software, and to permit persons to whom the Software is\n"           \
	"furnished to do so, subject to the following conditions:\n\n"                      \
	"The above copyright notice and this permission notice shall be included in all\n"  \
	"copies or substantial portions of the Software.\n\n"                               \
	"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"    \
	"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"        \
	"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n"     \
	"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"          \
	"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"   \
	"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"   \
	"SOFTWARE."

#define STREQ(str1, str2) (strcmp(str1, str2) == 0)

static void
print_version(void)
{
	printf("%s: %s (%s)\n%s\n", PROGRAM_NAME, DESCRIPTION, VERSION,
		MIT_LICENSE_TEXT);
	exit(0);
}

static void
print_help(void)
{
	printf("Usage: sv [OPTIONS]\n");
	printf("Options:\n");
	printf("\t-h, --help\t\t\tPrint this help message and exit\n");
	printf("\t-v, --version\t\t\tPrint the version message and exit\n");
	printf("\t--delay\t\t\t\tSet the delay of the program in milliseconds\n");
	printf("\t--bubble\t\t\tSort using the bubble sort algorithm\n");
	printf("\t--selection\t\t\tSort using the selection sort algorithm\n");
	printf("\t--insertion\t\t\tSort using the insertion sort algorithm\n");
	printf("\t--quick\t\t\t\tSort using the quicksort algorithm\n");
	exit(0);
}

/**
 * @brief Generate a random array of integers such that the smallest
 * value in the array is 0 and the largest is the size of the array
 * and that there are no repeated values.
 *
 * @param arr The array to randomize
 * @param array_size The size of the array
 */
static void
generate_random_array(int arr[], int array_size)
{
	srand(time(NULL));

	for (int i = 0; i < array_size; i++)
		arr[i] = i + 1;

	for (int i = array_size - 1; i > 0; i--)
	{
		int j	 = rand() % (i + 1);
		int temp = arr[i];
		arr[i]	 = arr[j];
		arr[j]	 = temp;
	}
}

/**
 * @brief Check if the user quit the program
 */
static void
check_event(void)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
		if (event.type == SDL_EVENT_QUIT)
		{
			SDL_Quit();
			exit(0);
		}
}

int
main(int argc, char **argv)
{
	const int column_width = 5;
	const int array_size   = 201;

	int delay = 25;

	enum algorithm algorithm = BUBBLE;

	if (argc > 1)
	{
		for (int i = 0; i < argc; i++)
		{
			const char *arg = argv[i];

			if (STREQ(arg, "-h") || STREQ(arg, "--help"))
				print_help();
			else if (STREQ(arg, "-v") || STREQ(arg, "--version"))
				print_version();
			else if (STREQ(arg, "--bubble"))
				algorithm = BUBBLE;
			else if (STREQ(arg, "--selection"))
				algorithm = SELECTION;
			else if (STREQ(arg, "--insertion"))
				algorithm = INSERTION;
			else if (STREQ(arg, "--quick"))
				algorithm = QUICK;

			if (i + 1 < argc)
			{
				const char *value = argv[i + 1];
				if (STREQ(arg, "--delay"))
					delay = atoi(value);
			}
		}
	}

	const int window_width	= array_size * column_width;
	const int window_height = window_width;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window	 *window   = SDL_CreateWindow(PROGRAM_NAME, window_width,
			window_height, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, 0);

	int array[array_size];
	generate_random_array(array, array_size);

	bool running = true;
	bool sorted	 = false;
	while (running)
	{
		check_event();

		if (!sorted)
		{
			switch (algorithm)
			{
			case BUBBLE:
				bubble_sort(renderer, array, array_size, column_width,
					window_width, window_height, delay);
				break;
			case SELECTION:
				selection_sort(renderer, array, array_size, column_width,
					window_width, window_height, delay);
				break;
			case INSERTION:
				insertion_sort(renderer, array, array_size, column_width,
					window_width, window_height, delay);
				break;
			case QUICK:
				quicksort(renderer, array, array_size, 0, array_size,
					column_width, window_width, window_height, delay);
				break;
			}
			sorted = true;
		}

		display_array(renderer, array, array_size, column_width, window_height,
			window_height, delay, sorted);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
