/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef BOID_H_
#define BOID_H_

#include <SDL3/SDL_rect.h>

struct boid
{
	SDL_FRect body;
	float	  vx, vy;
	float	  protected_range;
	float	  visual_range;
	float	  edge_margin;
	float	  separation_factor;
	float	  alignment_factor;
	float	  cohesion_factor;
	float	  edge_avoidance_factor;
	float	  max_speed;
	float	  min_speed;
};

/**
 * @brief Calculate the new position of a flock of boids
 *
 * @param boids The boids to steer
 * @param boids_count The amount of boids
 * @param window_width The width of the window displaying the boids
 * @param window_height The hight of the window displaying the boids
 */
void steer_boids(struct boid *boids, int boids_count, int window_width,
	int window_height);

#endif
