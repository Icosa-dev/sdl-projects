/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <SDL3/SDL.h>
#include <stdlib.h>
#include "snake.h"

/* Initialize the snake with values 0 and default direction UP */
struct snake *init_snake()
{
    struct snake *snake = malloc(sizeof(struct snake));
    snake->head = NULL;
    snake->tail = NULL;
    snake->size = 0;
    snake->direction = UP;

    return snake;
}

/* Push an SDL_FRect to the front of the snake deque */
void push_front(struct snake *snake, SDL_FRect rect)
{
    struct snake_node *new_node = malloc(sizeof(struct snake_node));
    new_node->rect = rect;
    new_node->next = snake->head;
    new_node->prev = NULL;

    if (snake->head != NULL)
        snake->head->prev = new_node;

    snake->head = new_node;

    if (snake->tail == NULL)
        snake->tail = new_node;

    snake->size++;
}

void pop_back(struct snake *snake)
{
    if (snake->head == NULL) return;

    if (snake->head == snake->tail)
    {
        free(snake->head);
        snake->head = NULL;
        snake->tail = NULL;
    } else  
    {
        struct snake_node *current = snake->head;
        while (current->next != snake->tail)
            current = current->next;

        free(snake->tail);
        snake->tail = current;
        snake->tail->next = NULL;
    }

    snake->size--;
}

SDL_FRect *get_segment(struct snake *snake, int index)
{
    if (index < 0 || index >= snake->size) return NULL;

    struct snake_node *current = snake->head;
    for (int i = 0; i < index; i++)
        current = current->next;

    return &current->rect;
}

void free_snake(struct snake *snake)
{
    if (snake == NULL) return;

    struct snake_node *current = snake->head;
    struct snake_node *next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }

    snake->head = NULL;
    snake->tail = NULL;
    snake->size = 0;
}
