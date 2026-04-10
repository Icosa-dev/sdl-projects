/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: MIT
 */

#include <SDL3/SDL.h>
#include <color.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PROGRAM_NAME "cgl"
#define DESCRIPTION "A basic implementation of Conway's Game of Life in SDL3"
#define AUTHORS "LJC"
#define VERSION "v0.1.0"
#define MIT_LICENSE_TEXT                                                                    \
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
	printf("\t--cell-size\t\t\tSet the size of a grid cell in pixels\n");
	printf("\t--rows\t\t\t\tSet the number of rows in the grid\n");
	printf("\t--columns\t\t\tSet the number of columns in the grid\n");
	printf("\t--delay\t\t\t\tSet the delay of the program in milliseconds\n");
	exit(0);
}

#define ALIVE true
#define DEAD false

/**
 * @brief Allocate a 2d array of booleans to the heap
 *
 * @param rows The number of rows in the array
 * @param columns The number of columns in the array
 * @return A 2d array of booleans as a pointer-pointer
 */
static bool **
alloc_grid(int rows, int columns)
{
	bool **grid = malloc(rows * sizeof(bool *));

	for (int i = 0; i < rows; i++)
	{
		grid[i] = calloc(columns, sizeof(bool));
	}

	return grid;
}

/**
 * @brief Free the memory for a grid of booleans
 *
 * @param grid The grid to free
 * @param rows The amount of rows in the grid
 */
static void
free_grid(bool **grid, int rows)
{
	for (int i = 0; i < rows; i++)
		free(grid[i]);
	free(grid);
}

/**
 * @brief Count the number of alive neighbors relative to a point on the grid
 *
 * @param grid The grid to check
 * @param rows The amount of rows in the grid
 * @param columns The amount of columns in the grid
 * @param r The row of the point to check neighbors for
 * @param c The column of the point to check neighbors for
 * @return int The number of alive neighbors relative to point [r][c]
 */
static int
count_neighbors(bool **grid, int rows, int columns, int r, int c)
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

			if (row >= 0 && row < rows && col >= 0 && col < columns)
			{
				if (grid[row][col] == ALIVE)
					count++;
			}
		}
	}

	return count;
}

/**
 * @brief Update a grid according to the rules of Conway's Game of Life
 *
 * @param grid The grid to update
 * @param rows The amount of rows in the grid
 * @param columns The amount of columns in the grid
 */
static void
update_grid(bool **grid, int rows, int columns)
{
	bool **tmp = alloc_grid(rows, columns);

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			int  count = count_neighbors(grid, rows, columns, r, c);
			bool current_cell = grid[r][c];

			/*
			 * RULES:
			 * 1) Death by Underpopulation: If the current cell is
			 * alive but has less than 2 alive neighbors it dies as
			 * if by underpopulation.
			 *
			 * 2) Death by Overpopulation: If the current cell is
			 * alive but has more than 3 alive neighbors it dies as
			 * if by overpopulation.
			 *
			 * 3) Reproduction: If the current cell is alive and has
			 * exactly 3 alive neighbors it becomes alive as if by
			 * reproduction.
			 *
			 * 4) Survival: If none of the above are met the state
			 * of the cell persists.
			 */
			if (current_cell == ALIVE && count < 2)
				tmp[r][c] =
					false; /* death by underpopulation */
			else if (current_cell == ALIVE && count > 3)
				tmp[r][c] = false; /* death by overpopulation */
			else if (current_cell == DEAD && count == 3)
				tmp[r][c] = true; /* reproduction */
			else
				tmp[r][c] = current_cell; /* survival */
		}
	}

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			grid[r][c] = tmp[r][c];
		}
	}

	free_grid(tmp, rows);
}

/**
 * @brief Generate random values for a grid
 *
 * @param grid The grid to randomize
 * @param rows The amount of rows in the grid
 * @param columns The amount of columns in the grid
 */
static void
generate_random_grid(bool **grid, int rows, int columns)
{
	srand(time(NULL));

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			grid[r][c] = rand() % 2;
		}
	}
}

int
main(int argc, char **argv)
{
	int cell_size = 5;
	int rows      = 200;
	int columns   = 200;

	int delay = 25;

	if (argc > 1)
	{
		for (int i = 0; i < argc; i++)
		{
			const char *arg = argv[i];

			if (STREQ(arg, "-h") || STREQ(arg, "--help"))
				print_help();
			else if (STREQ(arg, "-v") || STREQ(arg, "--version"))
				print_version();

			if (i + i < argc)
			{
				const char *value = argv[i + 1];

				if (STREQ(arg, "--cell-size"))
					cell_size = atoi(value);
				else if (STREQ(arg, "--rows"))
					rows = atoi(value);
				else if (STREQ(arg, "--columns"))
					columns = atoi(value);
				else if (STREQ(arg, "--delay"))
					delay = atoi(value);
			}
		}
	}

	int window_width  = rows * cell_size;
	int window_height = columns * cell_size;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window   *window   = SDL_CreateWindow(PROGRAM_NAME, window_width,
		    window_height, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

	bool **grid = alloc_grid(rows, columns);
	generate_random_grid(grid, rows, columns);

	bool	  running = true;
	SDL_Event event;
	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
				running = false;
		}

		update_grid(grid, rows, columns);

		SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(BLACK));
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(WHITE));
		for (int r = 0; r < rows; r++)
		{
			for (int c = 0; c < columns; c++)
			{
				float x = c * cell_size;
				float y = r * cell_size;

				if (grid[r][c] == ALIVE)
				{
					SDL_RenderFillRect(renderer,
						&(SDL_FRect) { x, y, cell_size,
							cell_size });
				}
			}
		}

		SDL_RenderPresent(renderer);
		SDL_Delay(delay);
	}

	free(grid);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
