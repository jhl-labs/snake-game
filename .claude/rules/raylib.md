---
paths:
  - "src/renderer.c"
  - "src/main.c"
---

# raylib Usage Rules

## Where raylib Can Be Used

### ALLOWED
- `renderer.c` - All rendering functions
- `main.c` - Input functions, window management

### FORBIDDEN
- `snake.c` - Model layer
- `food.c` - Model layer
- `game.c` - Logic layer

## Best Practices

### Color Constants
```c
static const Color SNAKE_COLOR = GREEN;
static const Color FOOD_COLOR = RED;
```

### Screen Dimensions
```c
#define WINDOW_WIDTH  (GRID_WIDTH * CELL_SIZE)
#define WINDOW_HEIGHT (GRID_HEIGHT * CELL_SIZE)
```

### Coordinate Conversion
```c
int pixel_x = grid_x * CELL_SIZE;
int pixel_y = grid_y * CELL_SIZE;
```

## Input Pattern

```c
// In main.c only
if (IsKeyPressed(KEY_UP)) {
    game_set_direction(&game, DIRECTION_UP);
}
```
