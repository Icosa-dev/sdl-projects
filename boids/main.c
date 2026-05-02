/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>
#include <color.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boid.h"

#define PROGRAM_NAME "boids"
#define DESCRIPTION                                                            \
    "A basic implementation of the boids swarm intelligence program in SDL3"
#define AUTHORS "LJC"
#define VERSION "v0.1.0"

#define STREQ(str1, str2) (strcmp(str1, str2) == 0)

static void printVersion(void) {
    printf("%s: %s (%s)\n", PROGRAM_NAME, DESCRIPTION, VERSION);
    exit(0);
}

static void printHelp(void) {
    printf("Usage: sv [OPTIONS]\n");
    printf("Options:\n");
    printf("\t-h, --help\t\t\tPrint this help message and exit\n");
    printf("\t-v, --version\t\t\tPrint the version message and exit\n");
    printf("\t--profile [birds, fish, insects]\n\t\t\t\t\tSet default values "
           "to reflect a kind of animal\n");
    printf("\t--boid-count <int>\t\tSet the number of boids to simulate\n");
    printf("\t--boid-size <int>\t\tSet the size of each boid in pixels\n");
    printf("\t--separation <int> \t\tSet the amount each boid should steer to "
           "avoid a flockmate\n");
    printf("\t--alignment <int> \t\tSet the amount each boid should steer "
           "towards the average direction of the flock\n");
    printf("\t--cohesion <int> \t\tSet the amount each boid should steer "
           "towards the center of the flock\n");
    printf("\t--protected-range <int>\t\tSet the distance at which the boid "
           "should avoid other boids\n");
    printf("\t--visual-range <int>\t\tSet the distance at which a boid can be "
           "affected by other boids\n");
    printf("\t--edge-avoidance <int>\t\tSet the amount each boid should avoid "
           "the edge of the screen\n");
    printf("\t--max-speed\t\t\tSet the max speed of the boids\n");
    printf("\t--min-speed\t\t\tSet the min speed of the boids\n");
    printf("\t--edge-margin\t\t\tSet the distance from the edge of the screen "
           "at which boids should steer away\n");
    exit(0);
}

/**
 * @brief Get a random position value
 *
 * @param max The max possible position
 * @return float A random position
 */
static float getRandPos(uint32_t max) { return SDL_randf() * max; }

/**
 * @brief Get a random velocity value
 *
 * @return float A random velocity
 */
static float getRandVel(void) { return SDL_randf() * 4 - 2; }

int main(int argc, char **argv) {
    const int windowWidth = 1000;
    const int windowHeight = 1000;

    int boidCount = 500;
    int boidSize = 5;
    float separationFactor = 0.05f;
    float alignmentFactor = 0.05f;
    float cohesionFactor = 0.0005f;
    int protectedRange = 8;
    int visualRange = 40;
    int edgeMargin = 50;
    float edgeAvoidanceFactor = 0.2f;
    float maxSpeed = 4.0f;
    float minSpeed = 2.0f;

    int delay = 16;

    if (argc > 1) {
        for (int i = 0; i < argc; i++) {
            const char *arg = argv[i];

            if (STREQ(arg, "-v") || STREQ(arg, "--version"))
                printVersion();
            else if (STREQ(arg, "-h") || STREQ(arg, "--help"))
                printHelp();

            if (i + 1 < argc) {
                const char *value = argv[i + 1];
                if (STREQ(arg, "--boid-count"))
                    boidCount = atoi(value);
                else if (STREQ(arg, "--profile")) {
                    if (STREQ(value, "fish")) {
                        /* Values for fish school
                         * simulation */
                        visualRange = 75;
                        cohesionFactor = 0.005;
                        alignmentFactor = 0.1;
                        protectedRange = 12;
                    } else if (STREQ(value, "insects")) {
                        /* Values for insect swarm
                         * simulation */
                        visualRange = 15;
                        maxSpeed = 5;
                        minSpeed = 4;
                        separationFactor = 0.2;
                        alignmentFactor = 0.01;
                    }
                } else if (STREQ(arg, "--boid-size"))
                    boidSize = atoi(value);
                else if (STREQ(arg, "--separation"))
                    separationFactor *= atof(value);
                else if (STREQ(arg, "--alignment"))
                    alignmentFactor *= atof(value);
                else if (STREQ(arg, "--cohesion"))
                    cohesionFactor *= atof(value);
                else if (STREQ(arg, "--protected-range"))
                    protectedRange *= atoi(value);
                else if (STREQ(arg, "--visual-range"))
                    visualRange *= atoi(value);
                else if (STREQ(arg, "--edge-avoidance"))
                    edgeAvoidanceFactor *= atof(value);
                else if (STREQ(arg, "--max-speed"))
                    maxSpeed *= atof(value);
                else if (STREQ(arg, "--min-speed"))
                    minSpeed *= atof(value);
                else if (STREQ(arg, "--edge-margin"))
                    edgeMargin = atoi(value);
                else if (STREQ(arg, "--delay"))
                    delay = atoi(value);
            }
        }
    }

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window =
        SDL_CreateWindow(PROGRAM_NAME, windowWidth, windowHeight, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    // init boids
    Boid *boids = (Boid *)malloc(boidCount * sizeof(Boid));
    for (int i = 0; i < boidCount; i++) {
        SDL_FRect body = (SDL_FRect){.x = getRandPos(windowWidth),
                                     .y = getRandPos(windowHeight),
                                     .w = boidSize,
                                     .h = boidSize};
        boids[i] = (Boid){.body = body,
                                 .vx = getRandVel(),
                                 .vy = getRandVel(),
                                 .protectedRange = protectedRange,
                                 .visualRange = visualRange,
                                 .edgeMargin = edgeMargin,
                                 .separationFactor = separationFactor,
                                 .alignmentFactor = alignmentFactor,
                                 .cohesionFactor = cohesionFactor,
                                 .edgeAvoidanceFactor = edgeAvoidanceFactor,
                                 .maxSpeed = maxSpeed,
                                 .minSpeed = minSpeed};
    }

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        // steer boids
        steerBoids(boids, boidCount, windowWidth, windowHeight);

        SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(BLACK));
        SDL_RenderClear(renderer);

        // render boids

        SDL_RenderPresent(renderer);
        SDL_Delay(delay);
    }

    free(boids);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
