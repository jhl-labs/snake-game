---
paths:
  - "src/**/*.c"
  - "include/**/*.h"
---

# Coding Style Rules

## Naming Conventions

### Functions
- Use `snake_case` for all function names
- Prefix with module name: `snake_move()`, `game_update()`, `food_spawn()`

### Types and Structs
- Use `PascalCase`: `Snake`, `Food`, `GameState`, `Direction`

### Constants
- Use `UPPER_SNAKE_CASE`: `GRID_WIDTH`, `MAX_SNAKE_LENGTH`, `CELL_SIZE`

### Variables
- Regular variables: `snake_case`
- Global variables: `g_` prefix (`g_game_state`)
- Static variables: `s_` prefix (`s_frame_count`)
- Pointer variables: `p_` prefix (`p_snake`)

## Code Structure

### Header Files
```c
#ifndef MODULE_H
#define MODULE_H

#include <stdbool.h>

typedef struct { } TypeName;

void function_name(void);

#endif // MODULE_H
```

### Source Files
```c
#include <stdlib.h>
#include "module.h"

static int s_counter = 0;

static void helper(void);

void function_name(void) { }
```

## Formatting

- Indent with 4 spaces (no tabs)
- Opening brace on same line
- Always use braces for if/for/while blocks
- Maximum line length: 100 characters
