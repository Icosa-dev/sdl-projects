/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <SDL3/SDL_rect.h>
#include <math.h>

#include "boid.h"

static float distToBoid(const Boid *boid1, const Boid *boid2) {
    float dy = boid2->body.y - boid1->body.y;
    float dx = boid2->body.x - boid1->body.x;

    return sqrtf(dx * dx + dy * dy);
}

static void calculateSeparation(Boid *boid, int idx, const Boid *boids,
                                int boidsCount) {
    float closeDx = 0;
    float closeDy = 0;

    for (int i = 0; i < boidsCount; i++) {
        if (i == idx)
            continue;

        const Boid *otherBoid = &boids[i];

        if (distToBoid(boid, otherBoid) < boid->protectedRange) {
            closeDx += boid->body.x - otherBoid->body.x;
            closeDy += boid->body.y - otherBoid->body.y;
        }
    }

    boid->vx += closeDx * boid->separationFactor;
    boid->vy += closeDy * boid->separationFactor;
}

static void calculate_alignment(Boid *boid, int idx, const Boid *boids,
                                int boidsCount) {
    float xvelAvg = 0;
    float yvelAvg = 0;
    int neighboringBoids = 0;

    for (int i = 0; i < boidsCount; i++) {
        if (i == idx)
            continue;

        const Boid *other_boid = &boids[i];

        if (distToBoid(boid, other_boid) < boid->visualRange) {
            xvelAvg += other_boid->vx;
            yvelAvg += other_boid->vy;
            neighboringBoids++;
        }
    }

    if (neighboringBoids > 0) {
        xvelAvg /= neighboringBoids;
        yvelAvg /= neighboringBoids;

        boid->vx += (xvelAvg - boid->vx) * boid->alignmentFactor;
        boid->vy += (yvelAvg - boid->vy) * boid->alignmentFactor;
    }
}

static void calculateCohesion(Boid *boid, int idx, const Boid *boids,
                              int boidsCount) {
    float xposAvg = 0;
    float yposAvg = 0;
    int neighboringBoids = 0;

    for (int i = 0; i < boidsCount; i++) {
        if (i == idx)
            continue;

        const Boid *otherBoid = &boids[i];

        if (distToBoid(boid, otherBoid) < boid->visualRange) {
            xposAvg += otherBoid->body.x;
            yposAvg += otherBoid->body.y;
            neighboringBoids++;
        }
    }

    if (neighboringBoids > 0) {
        xposAvg /= neighboringBoids;
        yposAvg /= neighboringBoids;

        boid->vx += (xposAvg - boid->body.x) * boid->cohesionFactor;
        boid->vy += (yposAvg - boid->body.y) * boid->cohesionFactor;
    }
}

static void calculateEdgeAvoidance(Boid *boid, int window_width,
                                   int window_height) {
    float leftMargin = boid->edgeMargin;
    float rightMargin = window_width - boid->edgeMargin;
    float topMargin = boid->edgeMargin;
    float bottomMargin = window_height - boid->edgeMargin;

    if (boid->body.x < leftMargin)
        boid->vx += boid->edgeAvoidanceFactor;

    if (boid->body.x > rightMargin)
        boid->vx -= boid->edgeAvoidanceFactor;

    if (boid->body.y > bottomMargin)
        boid->vy -= boid->edgeAvoidanceFactor;

    if (boid->body.y < topMargin)
        boid->vy += boid->edgeAvoidanceFactor;
}

void steerBoids(Boid *boids, int boidsCount, int windowWidth,
                int windowHeight) {
    for (int i = 0; i < boidsCount; i++) {
        Boid *boid = &boids[i];
        calculate_alignment(boid, i, boids, boidsCount);
        calculateSeparation(boid, i, boids, boidsCount);
        calculateCohesion(boid, i, boids, boidsCount);
        calculateEdgeAvoidance(boid, windowWidth, windowHeight);

        float speed = sqrt(boid->vx * boid->vx + boid->vy * boid->vy);

        if (speed > boid->maxSpeed) {
            boid->vx = (boid->vx / speed) * boid->maxSpeed;
            boid->vy = (boid->vy / speed) * boid->maxSpeed;
        }
        if (speed < boid->minSpeed) {
            boid->vx = (boid->vx / speed) * boid->minSpeed;
            boid->vy = (boid->vy / speed) * boid->minSpeed;
        }

        boid->body.x += boid->vx;
        boid->body.y += boid->vy;
    }
}
