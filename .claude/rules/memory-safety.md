---
paths:
  - "src/**/*.c"
---

# Memory Safety Rules

## Pointer Handling

### NULL Checks
Always check pointers at function entry:
```c
bool function_name(Type* p_param) {
    if (p_param == NULL) {
        fprintf(stderr, "Error: NULL pointer\n");
        return false;
    }
    // ... rest of function
}
```

### Pointer Prefix
- All pointer variables must use `p_` prefix

## Dynamic Memory

### Allocation Rules
- Always check allocation result
- Pair every `malloc`/`calloc` with `free`
- Set pointer to NULL after free

```c
Type* p_data = malloc(sizeof(Type));
if (p_data == NULL) {
    return false;
}
// ... use p_data ...
free(p_data);
p_data = NULL;
```

## Array Bounds

- Always validate array indices
- Use constants for array sizes
- Check bounds before access
