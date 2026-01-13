---
paths:
  - "test/**/*.c"
  - "exercises/**/*.c"
---

# Testing Rules

## Test File Naming
- Pattern: `test_<module>.c`
- Examples: `test_snake.c`, `test_game.c`

## Test Function Naming
- Pattern: `test_<function>_<scenario>()`
- Examples:
  - `test_snake_move_up()`
  - `test_snake_collision_wall()`

## Test Structure (AAA Pattern)

```c
void test_snake_move_up(void) {
    // Arrange
    Snake snake;
    snake_init(&snake, 5, 5);

    // Act
    snake_move(&snake, DIRECTION_UP);

    // Assert
    assert(snake.head_y == 4);
}
```

## Coverage Requirements

1. All public functions must have tests
2. Boundary conditions must be tested
3. Error cases must be tested
