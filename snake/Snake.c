/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>

// Grid values
#define CELL_SIZE 15
#define ROWS 50
#define COLUMNS 50

// Window values
#define TITLE "Snake"
#define WINDOW_WIDTH (COLUMNS * CELL_SIZE)
#define WINDOW_HEIGHT (ROWS * CELL_SIZE)
#define UPPER_BOUND_X WINDOW_WIDTH
#define UPPER_BOUND_Y WINDOW_HEIGHT
#define LOWER_BOUND_X (-CELL_SIZE)
#define LOWER_BOUND_Y (-CELL_SIZE)

// Snake deque can only be as large as the enitire grid
#define SNAKE_MAX_SIZE (ROWS * COLUMNS)
#define APPLES_MAX_SIZE 100

// Keybinds
#define KEYBIND_UP SDLK_W
#define KEYBIND_DOWN SDLK_S
#define KEYBIND_LEFT SDLK_A
#define KEYBIND_RIGHT SDLK_D

// Delay in ms
#define DELAY 50

typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

// Snake is a static deque of SDL_FRects
// it also contains the "direction" member
typedef struct
{
    SDL_FRect elements[SNAKE_MAX_SIZE];
    int head;
    int size;
    Direction direction;
} Snake;

void InitSnake(Snake *snake)
{
    snake->head = 0;
    snake->size = 0;
    snake->direction = UP;
}

void PushBack(Snake *snake, SDL_FRect rect)
{
    if (snake->size >= SNAKE_MAX_SIZE)
        return;

    snake->head = (snake->head - 1 + SNAKE_MAX_SIZE) % SNAKE_MAX_SIZE;
    snake->elements[snake->head] = rect;
    if (snake->size < SNAKE_MAX_SIZE)
        snake->size++;
}

void PopBack(Snake *snake)
{
    if (snake->size > 0)
        snake->size--;
}

SDL_FRect *GetSegment(Snake *snake, int index)
{
    int actualIndex = (snake->head + index) % SNAKE_MAX_SIZE;
    return &snake->elements[actualIndex];
}

float GetRandomX()
{
    return (float)(SDL_rand(COLUMNS) * CELL_SIZE);
}

float GetRandomY()
{
    return (float)(SDL_rand(ROWS) * CELL_SIZE);
}

void GameOverScreen(SDL_Renderer *renderer)
{
    SDL_Event event;
    for (;;)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                SDL_Quit();
                exit(0);
            }
        }
    }
}

int main(void)
{
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    // Create window and renderer
    SDL_Window *window = SDL_CreateWindow(TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    // Create snake
    Snake snake;
    InitSnake(&snake);
    PushBack(&snake, (SDL_FRect){WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, CELL_SIZE, CELL_SIZE});

    // Create apple
    SDL_FRect apple = (SDL_FRect){GetRandomX(), GetRandomY(), CELL_SIZE, CELL_SIZE};

    // Game loop
    bool running = true;
    SDL_Event event;
    while (running)
    {
        // Event handling
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                SDL_Quit();
                running = false;
            }

            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                switch (event.key.key)
                {
                case KEYBIND_UP:
                    if (snake.direction != DOWN)
                        snake.direction = UP;
                    break;
                case KEYBIND_DOWN:
                    if (snake.direction != UP)
                        snake.direction = DOWN;
                    break;
                case KEYBIND_LEFT:
                    if (snake.direction != RIGHT)
                        snake.direction = LEFT;
                    break;
                case KEYBIND_RIGHT:
                    if (snake.direction != LEFT)
                        snake.direction = RIGHT;
                    break;
                }
            }
        }

        SDL_FRect newHead = *GetSegment(&snake, 0);
        switch (snake.direction)
        {
        case UP:
            newHead.y -= CELL_SIZE;
            break;
        case DOWN:
            newHead.y += CELL_SIZE;
            break;
        case LEFT:
            newHead.x -= CELL_SIZE;
            break;
        case RIGHT:
            newHead.x += CELL_SIZE;
            break;
        }

        // Snake-apple collision detection
        bool ateApple = false;
        if (newHead.x == apple.x && newHead.y == apple.y)
        {
            ateApple = true;

            // NOTE: This does not check if the apple generates in the snake body
            apple.x = GetRandomX();
            apple.y = GetRandomY();
        }

        // Snake-border collision detection
        if (newHead.x == LOWER_BOUND_X || newHead.x == UPPER_BOUND_X
            || newHead.y == LOWER_BOUND_Y|| newHead.y == UPPER_BOUND_X)
            GameOverScreen(renderer);

        // Update snake body
        PushBack(&snake, newHead);
        if (!ateApple)
            PopBack(&snake); // remove back segment if not growing

        // Snake-snake collision detection
        for (int i = 1; i < snake.size; i++)
        {
            SDL_FRect *segment = GetSegment(&snake, i);
            if (newHead.x == segment->x && newHead.y == segment->y)
                GameOverScreen(renderer);
        }

        // Clear renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render snake
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int i = 0; i < snake.size; i++)
        {
            SDL_RenderFillRect(renderer, GetSegment(&snake, i));
        }

        // Render apple
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &apple);

        // Present rendered frame
        SDL_RenderPresent(renderer);
        SDL_Delay(DELAY);
    }

    return 0;
}
