/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef GRID_H
#define GRID_H

#include <stdbool.h>
#include "definitions.h"

#define ALIVE true
#define DEAD false

typedef bool grid[ROWS][COLUMNS];

/* Update the grid in accordance to the rules of Conway's Game of Life */
extern void update_grid(grid grid);

/* Generate a random grid */
extern void generate_random_grid(grid grid);

#endif
