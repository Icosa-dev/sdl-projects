/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef GRID_H
#define GRID_H

#include <stdbool.h>
#include "Definitions.h"

#define ALIVE true
#define DEAD false

typedef bool Grid[ROWS][COLUMNS];

// Update the grid in accordance to the rules of Conway's Game of Life
extern void UpdateGrid(Grid grid);

// Generate a random grid
extern void GenerateRandomGrid(Grid grid);

#endif
