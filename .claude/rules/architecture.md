---
paths:
  - "src/**/*.c"
  - "include/**/*.h"
---

# Architecture Rules

## MVC Pattern

This project follows a strict MVC (Model-View-Controller) pattern.

### Model Layer
**Files**: `snake.c`, `food.c`
- NO raylib dependency
- Pure data structures and manipulation
- No rendering logic

### Logic Layer
**Files**: `game.c`
- NO raylib dependency
- Game rules and state management
- Collision detection

### View Layer
**Files**: `renderer.c`
- raylib usage ALLOWED
- Only rendering logic
- No game state modification

### Controller Layer
**Files**: `main.c`
- raylib input functions ALLOWED
- Game loop management
- Input handling

## Dependency Direction

```
Controller (main.c)
    │
    ├──▶ Logic (game.c)
    │        │
    │        └──▶ Model (snake.c, food.c)
    │
    └──▶ View (renderer.c)
```

## Prohibited
- Model -> raylib (FORBIDDEN)
- Logic -> raylib (FORBIDDEN)
- Circular dependencies (FORBIDDEN)
