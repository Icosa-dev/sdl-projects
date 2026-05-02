/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef BOID_H
#define BOID_H

#include <SDL3/SDL_rect.h>

typedef struct {
    SDL_FRect body;
    float vx, vy;
    float protectedRange;
    float visualRange;
    float edgeMargin;
    float separationFactor;
    float alignmentFactor;
    float cohesionFactor;
    float edgeAvoidanceFactor;
    float maxSpeed;
    float minSpeed;
} Boid;

/**
 * @brief Calculate the new position of a flock of boids
 *
 * @param boids The boids to steer
 * @param boidsCount The amount of boids
 * @param windowWidth The width of the window displaying the boids
 * @param windowHeight The hight of the window displaying the boids
 */
void steerBoids(Boid *boids, int boidsCount, int windowWidth, int windowHeight);

#endif
