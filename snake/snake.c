/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <stdlib.h>

#include "snake.h"

struct snake *snake_init() {
    struct snake *snake = malloc(sizeof(struct snake));
    snake->head = NULL;
    snake->tail = NULL;
    snake->size = 0;
    snake->direction = UP;

    return snake;
}

void snake_push_front(struct snake *snake, SDL_FRect rect) {
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

void snake_pop_back(struct snake *snake) {
    if (snake->head == NULL)
        return;

    if (snake->head == snake->tail) {
        free(snake->head);
        snake->head = NULL;
        snake->tail = NULL;
    } else {
        struct snake_node *current = snake->head;
        while (current->next != snake->tail)
            current = current->next;

        free(snake->tail);
        snake->tail = current;
        snake->tail->next = NULL;
    }

    snake->size--;
}

SDL_FRect *snake_get_segment(const struct snake *snake, size_t index) {
    if (index >= snake->size)
        return NULL;

    struct snake_node *current = snake->head;
    for (size_t i = 0; i < index; i++)
        current = current->next;

    return &current->rect;
}

void snake_free(struct snake *snake) {
    if (snake == NULL)
        return;

    struct snake_node *current = snake->head;
    struct snake_node *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    snake->head = NULL;
    snake->tail = NULL;
    snake->size = 0;
}
