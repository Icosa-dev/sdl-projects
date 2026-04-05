/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "grid.h"
#include "definitions.h"

static int count_neighbors(grid_t grid, int r, int c)
{
    int count = 0;

    // Index around the cell (-1, 0, +1)
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            // If i and j are 0 then the current neighbor
            // being checked is the current cell. Skip.
            if (i == 0 && j == 0)
                continue;

            // The neighbor being checked is offset from the
            // current cell by i and j
            int row = r + i;
            int col = c + j;

            // if neighbor is not off the grid
            if (row >= 0 && row < ROWS && col >= 0 && col < COLUMNS)
            {
                // If the neighbor is alive add to count
                if (grid[row][col] == ALIVE)
                    count++;
            }
        }
    }

    return count;
}

void update_grid(grid_t grid)
{
    /* Hold next generation of cells */
    grid_t tmp;

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLUMNS; c++)
        {
            int count = count_neighbors(grid, r, c);
            bool current_cell = grid[r][c];

            /*
             * RULES:
             * 1) Death by Underpopulation: If the current cell is alive but has less than
             *    2 alive neighbors it dies as if by underpopulation.
             *
             * 2) Death by Overpopulation: If the current cell is alive but has more than
             *    3 alive neighbors it dies as if by overpopulation.
             *
             * 3) Reproduction: If the current cell is alive and has exactly 3 alive neighbors
             *    it becomes alive as if by reproduction.
             *
             * 4) Survival: If none of the above are met the state of the cell persists.
             */
            if (current_cell == ALIVE && count < 2)
                tmp[r][c] = false; /* death by underpopulation */
            else if (current_cell == ALIVE && count > 3)
                tmp[r][c] = false; /* death by overpopulation */
            else if (current_cell == DEAD && count == 3)
                tmp[r][c] = true; /* reproduction */
            else
                tmp[r][c] = current_cell; /* survival */
        }
    }

    /* Copy the temporary grid into the actual board */
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLUMNS; c++)
        {
            grid[r][c] = tmp[r][c];
        }
    }
}

/* Generate a random grid */
void generate_random_grid(grid_t grid)
{
    srand(time(NULL));

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLUMNS; c++)
        {
            grid[r][c] = rand() % 2;
        }
    }
}
