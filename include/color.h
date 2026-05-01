/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef COLOR_H_
#define COLOR_H_

#include <stdint.h>

struct color {
    uint8_t r, g, b, a;
};

#define COLOR_TO_ARGS(color) color.r, color.g, color.b, color.a

#define BLACK (struct color){0, 0, 0, 255}
#define WHITE (struct color){255, 255, 255, 255}
#define RED (struct color){255, 0, 0, 255}
#define GREEN (struct color){0, 255, 0, 255}
#define BLUE (struct color){0, 0, 255, 255}

#endif
