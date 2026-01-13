---
paths:
  - "src/**/*.c"
---

# Error Handling Rules

## Function Return Values

### Boolean Returns
- `true` = success
- `false` = failure

### Error Messages
- Use `fprintf(stderr, ...)` for errors
- Include function name in error message

```c
fprintf(stderr, "snake_init: Invalid position (%d, %d)\n", x, y);
```

## Constants

### No Magic Numbers
```c
// BAD
if (snake->length > 100) { }

// GOOD
if (snake->length > MAX_SNAKE_LENGTH) { }
```

### Definition Location
- Game-wide constants: `game_constants.h`
- Module-specific: module header file

## Debug Assertions

```c
#include <assert.h>

void process_game(Game* p_game) {
    assert(p_game != NULL);  // Debug only
}
```
