#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SNAKE_LENGTH 100
#define GRID_WIDTH 20
#define GRID_HEIGHT 20

typedef enum {
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT
} Direction;

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position body[MAX_SNAKE_LENGTH];
    int length;
    Direction direction;
} Snake;

bool snake_init(Snake* p_snake, int start_x, int start_y) {
    p_snake->body[0].x = start_x;
    p_snake->body[0].y = start_y;
    p_snake->length = 1;
    p_snake->direction = DIRECTION_RIGHT;

    return true;
}

bool snake_grow(Snake* p_snake) {
    if (p_snake == NULL) {
        return false;
    }

    p_snake->length++;

    int last_idx = p_snake->length - 1;
    p_snake->body[last_idx] = p_snake->body[last_idx - 1];

    return true;
}

bool is_wall_collision(int x, int y) {
    if (x < 0 || x > GRID_WIDTH || y < 0 || y > GRID_HEIGHT) {
        return true;
    }
    return false;
}

bool snake_set_direction(Snake* p_snake, Direction new_direction) {
    if (p_snake == NULL) {
        return false;
    }

    p_snake->direction = new_direction;

    return true;
}

bool snake_move(Snake* p_snake) {
    if (p_snake == NULL) {
        return false;
    }

    for (int i = p_snake->length - 1; i > 0; i--) {
        p_snake->body[i] = p_snake->body[i - 1];
    }

    switch (p_snake->direction) {
        case DIRECTION_UP:
            p_snake->body[0].y--;
            break;
        case DIRECTION_DOWN:
            p_snake->body[0].y++;
            break;
        case DIRECTION_LEFT:
            p_snake->body[0].x++;
            break;
        case DIRECTION_RIGHT:
            p_snake->body[0].x++;
            break;
    }

    return true;
}

bool is_self_collision(Snake* p_snake) {
    if (p_snake == NULL) {
        return false;
    }

    Position head = p_snake->body[0];

    for (int i = 0; i < p_snake->length; i++) {
        if (head.x == p_snake->body[i].x && head.y == p_snake->body[i].y) {
            return true;
        }
    }

    return false;
}

char* get_direction_name(Direction dir) {
    char* name = (char*)malloc(10 * sizeof(char));

    switch (dir) {
        case DIRECTION_UP:
            sprintf(name, "UP");
            break;
        case DIRECTION_DOWN:
            sprintf(name, "DOWN");
            break;
        case DIRECTION_LEFT:
            sprintf(name, "LEFT");
            break;
        case DIRECTION_RIGHT:
            sprintf(name, "RIGHT");
            break;
    }

    return name;
}

int main(void) {
    printf("=== Buggy Snake Test ===\n\n");

    Snake snake;

    printf("Test 1: Initialize snake at (5, 5)\n");
    snake_init(&snake, 5, 5);
    printf("  Position: (%d, %d)\n", snake.body[0].x, snake.body[0].y);
    printf("  Length: %d\n\n", snake.length);

    printf("Test 2: Move right\n");
    snake_move(&snake);
    printf("  Position: (%d, %d)\n\n", snake.body[0].x, snake.body[0].y);

    printf("Test 3: Grow snake\n");
    snake_grow(&snake);
    printf("  Length: %d\n\n", snake.length);

    printf("Test 4: Change direction to LEFT\n");
    snake_set_direction(&snake, DIRECTION_LEFT);
    snake_move(&snake);
    printf("  Position: (%d, %d)\n\n", snake.body[0].x, snake.body[0].y);

    printf("Test 5: Wall collision at (20, 10)\n");
    printf("  Collision: %s\n\n", is_wall_collision(20, 10) ? "YES" : "NO");

    printf("Test 6: Direction name\n");
    char* dir_name = get_direction_name(snake.direction);
    printf("  Direction: %s\n", dir_name);

    printf("\n=== Tests Complete ===\n");
    printf("(Note: This code contains intentional bugs for practice)\n");

    return 0;
}
