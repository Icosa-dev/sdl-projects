/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: MIT
 */

#include <SDL3/SDL_rect.h>
#include <math.h>

#include "boid.h"

static float
dist_to_boid(const struct boid *boid1, const struct boid *boid2)
{
	float dy = boid2->body.y - boid1->body.y;
	float dx = boid2->body.x - boid1->body.x;

	return sqrtf(dx * dx + dy * dy);
}

static void
calculate_separation(struct boid *boid, int idx, const struct boid *boids,
	int boids_count)
{
	float close_dx = 0;
	float close_dy = 0;

	for (int i = 0; i < boids_count; i++)
	{
		if (i == idx)
			continue;

		const struct boid *other_boid = &boids[i];

		if (dist_to_boid(boid, other_boid) < boid->protected_range)
		{
			close_dx += boid->body.x - other_boid->body.x;
			close_dy += boid->body.y - other_boid->body.y;
		}
	}

	boid->vx += close_dx * boid->separation_factor;
	boid->vy += close_dy * boid->separation_factor;
}

static void
calculate_alignment(struct boid *boid, int idx, const struct boid *boids,
	int boids_count)
{
	float xvel_avg = 0;
	float yvel_avg = 0;
	int neighboring_boids = 0;

	for (int i = 0; i < boids_count; i++)
	{
		if (i == idx)
			continue;

		const struct boid *other_boid = &boids[i];

		if (dist_to_boid(boid, other_boid) < boid->visual_range)
		{
			xvel_avg += other_boid->vx;
			yvel_avg += other_boid->vy;
			neighboring_boids++;
		}
	}

	if (neighboring_boids > 0)
	{
		xvel_avg /= neighboring_boids;
		yvel_avg /= neighboring_boids;

		boid->vx += (xvel_avg - boid->vx) * boid->alignment_factor;
		boid->vy += (yvel_avg - boid->vy) * boid->alignment_factor;
	}
}

static void
calculate_cohesion(struct boid *boid, int idx, const struct boid *boids,
	int boids_count)
{
	float xpos_avg = 0;
	float ypos_avg = 0;
	int neighboring_boids = 0;

	for (int i = 0; i < boids_count; i++)
	{
		if (i == idx)
			continue;

		const struct boid *other_boid = &boids[i];

		if (dist_to_boid(boid, other_boid) < boid->visual_range)
		{
			xpos_avg += other_boid->body.x;
			ypos_avg += other_boid->body.y;
			neighboring_boids++;
		}
	}

	if (neighboring_boids > 0)
	{
		xpos_avg /= neighboring_boids;
		ypos_avg /= neighboring_boids;

		boid->vx += (xpos_avg - boid->body.x) * boid->cohesion_factor;
		boid->vy += (ypos_avg - boid->body.y) * boid->cohesion_factor;
	}
}

static void
calculate_edge_avoidance(struct boid *boid, int window_width, int window_height)
{
	float left_margin = boid->edge_margin;
	float right_margin = window_width - boid->edge_margin;
	float top_margin = boid->edge_margin;
	float bottom_margin = window_height - boid->edge_margin;

	if (boid->body.x < left_margin)
		boid->vx += boid->edge_avoidance_factor;

	if (boid->body.x > right_margin)
		boid->vx -= boid->edge_avoidance_factor;

	if (boid->body.y > bottom_margin)
		boid->vy -= boid->edge_avoidance_factor;

	if (boid->body.y < top_margin)
		boid->vy += boid->edge_avoidance_factor;
}

void
steer_boids(struct boid *boids, int boids_count, int window_width,
	int window_height)
{
	for (int i = 0; i < boids_count; i++)
	{
		struct boid *boid = &boids[i];
		calculate_alignment(boid, i, boids, boids_count);
		calculate_separation(boid, i, boids, boids_count);
		calculate_cohesion(boid, i, boids, boids_count);
		calculate_edge_avoidance(boid, window_width, window_height);

		float speed = sqrt(boid->vx * boid->vx + boid->vy * boid->vy);

		if (speed > boid->max_speed)
		{
			boid->vx = (boid->vx / speed) * boid->max_speed;
			boid->vy = (boid->vy / speed) * boid->max_speed;
		}
		if (speed < boid->min_speed)
		{
			boid->vx = (boid->vx / speed) * boid->min_speed;
			boid->vy = (boid->vy / speed) * boid->min_speed;
		}

		boid->body.x += boid->vx;
		boid->body.y += boid->vy;
	}
}
