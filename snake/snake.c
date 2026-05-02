/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <stdlib.h>

#include "snake.h"

Snake *snakeInit(void) {
    Snake *snake = malloc(sizeof(Snake));
    snake->head = NULL;
    snake->tail = NULL;
    snake->size = 0;
    snake->direction = UP;

    return snake;
}

void snakePushFront(Snake *snake, SDL_FRect rect) {
    SnakeNode *newNode = malloc(sizeof(SnakeNode));
    newNode->rect = rect;
    newNode->next = snake->head;
    newNode->prev = NULL;

    if (snake->head != NULL)
        snake->head->prev = newNode;

    snake->head = newNode;

    if (snake->tail == NULL)
        snake->tail = newNode;

    snake->size++;
}

void snakePopBack(Snake *snake) {
    if (snake->head == NULL)
        return;

    if (snake->head == snake->tail) {
        free(snake->head);
        snake->head = NULL;
        snake->tail = NULL;
    } else {
        SnakeNode *current = snake->head;
        while (current->next != snake->tail)
            current = current->next;

        free(snake->tail);
        snake->tail = current;
        snake->tail->next = NULL;
    }

    snake->size--;
}

SDL_FRect *snakeGetSegment(const Snake *snake, int index) {
    if (index >= snake->size)
        return NULL;

    SnakeNode *current = snake->head;
    for (int i = 0; i < index; i++)
        current = current->next;

    return &current->rect;
}

void snakeFree(Snake *snake) {
    if (snake == NULL)
        return;

    SnakeNode *current = snake->head;
    SnakeNode *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    snake->head = NULL;
    snake->tail = NULL;
    snake->size = 0;
}
