/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef GRID_H
#define GRID_H

#include <stdbool.h>
#include "definitions.h"

#define ALIVE true
#define DEAD false

typedef bool grid_t[ROWS][COLUMNS];

/* Update the grid in accordance to the rules of Conway's Game of Life */
extern void update_grid(grid_t grid);

/* Generate a random grid */
extern void generate_random_grid(grid_t grid);

#endif
