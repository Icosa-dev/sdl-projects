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

static void print_version(void) {
    printf("%s: %s (%s)\n", PROGRAM_NAME, DESCRIPTION, VERSION);
    exit(0);
}

static void print_help(void) {
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
static float get_rand_pos(uint32_t max) { return SDL_randf() * max; }

/**
 * @brief Get a random velocity value
 *
 * @return float A random velocity
 */
static float get_rand_vel(void) { return SDL_randf() * 4 - 2; }

int main(int argc, char **argv) {
    const uint32_t window_width = 1000;
    const uint32_t window_height = 1000;

    size_t boid_count = 500;
    uint32_t boid_size = 5;
    float separation_factor = 0.05;
    float alignment_factor = 0.05;
    float cohesion_factor = 0.0005;
    uint32_t protected_range = 8;
    uint32_t visual_range = 40;
    float edge_avoidance_factor = 0.2;
    float max_speed = 4;
    float min_speed = 2;
    uint32_t edge_margin = 50;

    uint32_t delay = 16;

    if (argc > 1) {
        for (int i = 0; i < argc; i++) {
            const char *arg = argv[i];

            if (STREQ(arg, "-v") || STREQ(arg, "--version"))
                print_version();
            else if (STREQ(arg, "-h") || STREQ(arg, "--help"))
                print_help();

            if (i + 1 < argc) {
                const char *value = argv[i + 1];
                if (STREQ(arg, "--boid-count"))
                    boid_count = atoi(value);
                else if (STREQ(arg, "--profile")) {
                    if (STREQ(value, "fish")) {
                        /* Values for fish school
                         * simulation */
                        visual_range = 75;
                        cohesion_factor = 0.005;
                        alignment_factor = 0.1;
                        protected_range = 12;
                    } else if (STREQ(value, "insects")) {
                        /* Values for insect swarm
                         * simulation */
                        visual_range = 15;
                        max_speed = 5;
                        min_speed = 4;
                        separation_factor = 0.2;
                        alignment_factor = 0.01;
                    }
                } else if (STREQ(arg, "--boid-size"))
                    boid_size = atoi(value);
                else if (STREQ(arg, "--separation"))
                    separation_factor *= atof(value);
                else if (STREQ(arg, "--alignment"))
                    alignment_factor *= atof(value);
                else if (STREQ(arg, "--cohesion"))
                    cohesion_factor *= atof(value);
                else if (STREQ(arg, "--protected-range"))
                    protected_range *= atoi(value);
                else if (STREQ(arg, "--visual-range"))
                    visual_range *= atoi(value);
                else if (STREQ(arg, "--edge-avoidance"))
                    edge_avoidance_factor *= atof(value);
                else if (STREQ(arg, "--max-speed"))
                    max_speed *= atof(value);
                else if (STREQ(arg, "--min-speed"))
                    min_speed *= atof(value);
                else if (STREQ(arg, "--edge-margin"))
                    edge_margin = atoi(value);
                else if (STREQ(arg, "--delay"))
                    delay = atoi(value);
            }
        }
    }

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window =
        SDL_CreateWindow(PROGRAM_NAME, window_width, window_height, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    struct boid *boids =
        (struct boid *)malloc(boid_count * sizeof(struct boid));
    for (size_t i = 0; i < boid_count; i++) {
        SDL_FRect body = (SDL_FRect){.x = get_rand_pos(window_width),
                                     .y = get_rand_pos(window_height),
                                     .w = boid_size,
                                     .h = boid_size};
        boids[i] = (struct boid){.body = body,
                                 .vx = get_rand_vel(),
                                 .vy = get_rand_vel(),
                                 .protected_range = protected_range,
                                 .visual_range = visual_range,
                                 .edge_margin = edge_margin,
                                 .separation_factor = separation_factor,
                                 .alignment_factor = alignment_factor,
                                 .cohesion_factor = cohesion_factor,
                                 .edge_avoidance_factor = edge_avoidance_factor,
                                 .max_speed = max_speed,
                                 min_speed = min_speed};
    }

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        steer_boids(boids, boid_count, window_width, window_height);

        SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(BLACK));
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, COLOR_TO_ARGS(WHITE));
        for (size_t i = 0; i < boid_count; i++) {
            SDL_RenderFillRect(renderer, &boids[i].body);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(delay);
    }

    free(boids);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
